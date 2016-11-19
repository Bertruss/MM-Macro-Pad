/*3x3 matrix control program
 * Due to current environment constraints, code is currently compiled via 
 * Arduino with the teensyduino libraries. Hope to move to compiling c for the 
 * teensy via make.
 */

//LCqueue available in seperate repo. 
#include <LCqueue.h>

//         *debounce
//               - cleans input of noise that occurs from switch depressing.
//               - not sure if this is truly necessary, but on a faster chip with higher polling speed it migh make a difference 
//         *button cooldown 
//               - prevents a button press from being registered multiple times
//               - disabled by setting "cooldownt" to 0
//               - when you disable the cooldown timer, button presses are still only registered once assuming it passes debounce
//         *longhold 
//               - you know when you hold a button for more than a second it just acts like continuous presses. that thing.
//               - adjusted via "longholdt" 
//               - Can't really be disabled, but you can set the number really high and that would effectively do the trick.  
//
//          note: all time is counted in ms, or 1/1000 of a second

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
unsigned long confirm_timer[9]; //how long since the button reached confirmed on or off state
unsigned long debounce_timer[9]; //how long since the buttons state last changed
const unsigned long delayt = 20;//how long a button needs to be depressed in order to be read, in ms

//output/operation queue
LCqueue *outputBuffer; 

//cooldown timer, in case you want to prevent a button from being able to be pressed consecutively too quickly.
unsigned long cooldown_timer[9];
const unsigned long cooldownt PROGMEM = 100;

//longhold timer, to prevent unnecessary numbers of consecutive registered button operations.  
unsigned long longhold_timer[9]; //timer for longhold state limits how quickly longhold triggers a registered activation 
const unsigned long longholdt PROGMEM = 1000;//time to longhold state
const unsigned long longhold_active_t PROGMEM = 200;//limits how quickly longhold signals an output

//given col and row, returns button address and "number"
int buttonaddr(int rownum, int colnum){
  return (3 * rownum) + (colnum);
  }

//debouncing pin reader, prevents rapid on/off switching or registering multiple button presses due to switch noise. 
void debounce(int rownum, int colnum){
    int address = buttonaddr(rownum, colnum);
    
    //current state of pin assigned to pinRead
    int readPin = digitalRead(col[colnum]); 
    
    //if pin state changes, reset debounce time marker to current time
    if(readPin != button_lastState[address]){
      debounce_timer[address] = millis();
      }

    //if pin has not changed state for longer than the debounce delay (delayt), then commit current state to button_buff  
    if((millis() - debounce_timer[address]) > delayt && readPin != button_buff[address]){ 
        confirm_timer[address] = millis();
        button_buff[address] = readPin;
      }
    button_lastState[address] = readPin;
  }

bool checkLongHold(unsigned long button_cooldown_time,  unsigned long button_confirmed_state_change_time, int address){
     /* if the last time buttonstate changed was before the last time the cooldown timer was reset, continues. 
      * ">=" because it happens on a shorten than ms scale, but if they're equal, cooldown reset had to happen 
      *  after the state change reset because of the order of the flow. this next part is bronken up to be 
      *  easier to explain and read.   
      */
     if(button_cooldown_time >= button_confirmed_state_change_time){

      //if the button has been held down long enough for a long hold mode activation
      if((millis() - button_cooldown_time) > longholdt){

          //this is another timer checker used to limit how fast the longhold state registers activations
          if((millis() - longhold_timer[address]) > longhold_active_t){
          longhold_timer[address] = millis();
          return true;
        }
      }
    }
    return false;
  }


//multistage checker that handles cooldown and longhold functionality logic
bool checkCoolDown(int address){//returns true if cooldown timer is exceeded, or if longhold state is reached
    unsigned long button_cooldown_time = cooldown_timer[address];
    unsigned long button_confirmed_state_change_time = confirm_timer[address];
    if(checkLongHold(button_cooldown_time, button_confirmed_state_change_time, address)){
      return true;
      }
    else if((millis() - button_cooldown_time) > cooldownt && button_confirmed_state_change_time > button_cooldown_time && cooldownt != 0){//if cooldown timer is exceeded, resets timer, returns true //&& button_confirmed_state_change_time > button_cooldown_time 
      cooldown_timer[address] = millis();
      return true;
    }                             
    else if (button_confirmed_state_change_time > button_cooldown_time){//makes sure that if the cooldown is disabled, one button press still only registers once unless longhold is exceeded
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


//reads the current button state as determined via debounced matrix scan, 
//and then makes sure the press is registerable (has no withstanding cooldown, if it's exceeded longhold time, or if the ouput buffer is full) 
void genOutputBuffer(){
  int cnt;
  for(cnt = 0; cnt < 9; cnt = cnt + 1){
    int state = button_buff[cnt];
    if(state == HIGH && checkCoolDown(cnt) && (count(outputBuffer) < 100)){
      push(outputBuffer, cnt);
      } 
    }
  }

//executes operations described by the output.
//NOTE:
//This part will be expanded to execute macro's, perhaps described in an imported c file.  
void executeBuffer(){
    int action = pop(outputBuffer);
    if(action != -1){
    Keyboard.print(action);
    }
  }
  
void setup() {
  int cnt;
  //button matrix instantiation
  for (cnt = 0; cnt < 3; cnt = cnt + 1){ //instantiates every pin in row[] to OUTPUT and in col[] to INPUT_PULLUP
      pinMode(row[cnt],OUTPUT);
      pinMode(col[cnt],INPUT_PULLUP);
    }
  outputBuffer = new_queue(); //allocating memory for queue
}

void loop() {
  scan();
  genOutputBuffer();
  executeBuffer();
}
