//QueueArray library is required for this implementation. The QueueList library...just made the LED blink furiously. This can be found here: http://playground.arduino.cc/Code/QueueArray
#include <QueueArray.h> //NOT WORKING

//9 button matrix with 
//         *debounce
//               - cleans input of noise that occurs from switch depressing.
//               - not sure if this is truly necessary, but on a faster chip with higher polling speed it migh make a difference 
//         *button cooldown 
//               - prevents a button press from being registered multiple times
//               - disabled by setting "cooldownt" to 0
//         *longhold 
//               - you know when you hold a button for more than a second it just acts like continuous presses. that thing.
//               - adjusted via "longholdt" 
//               - Can't really be disabled, but you can set the number really high and that would effectively do the trick.  

 

// pin assignment
//columns and rows numbered 0, 1, 2
const int col[3] PROGMEM = {1, 2, 3};
const int row[3] PROGMEM = {9, 10, 11};

// You can map the keys however you wish, 
// but this is how it is configured by default
//
//       the number on each key in the map
//       below marks its index in the buffer
//       as well as it's signifier in the
//       operation queue 
//
//               0    1    2      
//            |----|----|----|
//          0 | 0  | 1  | 2  |
//            |----|----|----|
//          1 | 3  | 4  | 5  |
//            |----|----|----|
//          2 | 6  | 7  | 8  |
//            |----|----|----|


//arrays for button debouncing
int button_buff[9];
int button_lastState[9] = {0,0,0,0,0,0,0,0,0};
unsigned long debounce_timer[9];
const unsigned long delayt = 20;//how long a button needs to be depressed in order to be read, in ms

//output/operation queue
QueueArray<int> outputBuffer; //NOT WORKING

//longhold timer, to prevent unnecessary numbers of consecutive registered button operations.  
unsigned long cooldown_timer[9];
const unsigned long cooldownt PROGMEM = 200;
const unsigned long longholdt PROGMEM = 1000;

//given col and row, returns button address and "number"
int buttonaddr(int rownum, int colnum){
  return (3 * rownum) + (colnum);
  }

//debouncing pin reader, prevents rapid on/off switching or registering multiple button presses due to switch noise. 
void debounce(int rownum, int colnum){
    int address = buttonaddr(rownum, colnum);
    int readPin = digitalRead(col[colnum]); //current state of pin assigned to pinRead
    
    if(readPin != button_lastState[address]){//if pin state changes, reset debounce time marker to current time
      debounce_timer[address] = millis();
      }
      
    if((millis() - debounce_timer[address]) > delayt && readPin != button_buff[address]){ //if pin has not changed state for longer than the debounce delay (delayt), then commit current state to button_buff
        button_buff[address] = readPin;
      }
    button_lastState[address] = readPin;
  }

bool checkCoolDown(int address){//returns true if cooldown timer is exceeded, or if longhold state is reached
    if(cooldown_timer[address] > debounce_timer[address])//if the last time buttonstate changed was before the last time the cooldown timer was reset  
    {
      if((millis() - cooldown_timer[address]) > longholdt){ //and since last cooldown timer update,  "longhold" time has been exceeded, return true
        return true;
      }
      else{
        return false;
      }  
    }
    else if((millis() - cooldown_timer[address]) > cooldownt){//if cooldown timer is exceeded, resets timer, returns true 
      cooldown_timer[address] = millis();
      return true;
    }                             
    else{
      return false; //all else return false.
    }                                                          
  }
  
//sets chosen row high and all others low
void rowSet(int rowIn) { 
   int cnt;
   for(cnt = 0; cnt < 3; cnt = cnt + 1){
     if(cnt == rowIn){
      digitalWrite(row[cnt], HIGH);
      }
     else{
      digitalWrite(row[cnt], LOW);
      }
     }
   }


//checks every button in the matrix, runs "debounce" on it 
void scan(){
    //numbered by array index 0 to two. 
    int rownum, colnum;
    for(rownum = 0; rownum < 3; rownum = rownum + 1){   //increments rownum 
      rowSet(rownum);                                   //and sets the row with that number to HIGH.
      for(colnum = 0; colnum < 3; colnum = colnum + 1){ //increments colnum 
        debounce(rownum, colnum);                       //and runs "debounce" on that column
      }
    }
  }


void genOutputBuffer(){
  int cnt;
  for(cnt = 0; cnt < 9; cnt = cnt + 1){
    int state =  button_buff[cnt];  
    if(state == HIGH && checkCoolDown(cnt) && (outputBuffer.count() < 10)){
      outputBuffer.enqueue(cnt);
      } 
    }
  }

//executes operations described by the output.
//NOTE:
//This part will be expanded to execute macro's, perhaps described in an imported c file. 
//Only executes one at a time to make sure data throughput isn't overwhelmed.
//This will obviously lead to some issues (output buffer overflow, potential input/output lag(?), but I don't know solutions to these yet.
//perhaps it should run 5 ouputs every loop. this would be over half of the potential inputs registered in any given loop, 
//and add a delay of one ms to every execution to /marginally/ slow down the output   
void executeBuffer(){
  int action = outputBuffer.dequeue();//NOT WORKING
  Keyboard.print(action);
  }
  
void setup() {
  int cnt;
  //button matrix instantiation
  for (cnt = 0; cnt < 3; cnt = cnt + 1){ //instantiates every pin in row[] to OUTPUT and in col[] to INPUT_PULLUP
      pinMode(row[cnt],OUTPUT);
      pinMode(col[cnt],INPUT_PULLUP);
    }
}

void loop() {
  scan();
  genOutputBuffer();
  delay(10);
  executeBuffer();
}


