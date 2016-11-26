#include <LCqueue.h>
#include <usb_keyboard.h>

//9 button matrix with 
//         *debounce
//               - cleans input of noise that occurs from switch depressing.
//               - not sure if this is truly necessary, but on a faster chip with higher polling speed it migh make a difference 
//         *button cooldown 
//               - prevents a button press from being registered multiple times
//               - Independent values for each button, disabled by setting "cooldownt" to 0
//               - when you disable the cooldown timer, button presses are still only registered once assuming it passes debounce
//         *longhold 
//               - you know when you hold a button for more than a second it just acts like continuous presses. that thing.
//               - adjusted via "longholdt" 
//               - Independent values for each button
//         *Media Keys
//               -added functions for easy mapping of media keys
//         *****fully rebindable*****
//              - each key is read as their address, that address is then mapped to a case in the keymap function, 
//                which essentially executes as a script. For a full range of what can be put in these scripts, documentation  
//                from PJRC (the teensyduino libs) and Arduino regarding keyboard communications and the signals that can be sent from the teensy
//              
//         ***** extensible *****
//                - many parts will have to be manually adjusted to account, but I have tried to make it as painless as possible to
//                  make your own matrix with however many rows and columns as you might want.
//
//          note: all time is counted in ms, or 1/1000 of a second
//
//
//
//
//    You can map the keys however you wish, 
//    but this is how it is configured by default
//
//          the number on each key in the map
//          below marks its index in the buffer
//          as well as it's signifier in the
//           operation queue 
//
//                  0    1    2      
//               |----|----|----|
//             0 | 0  | 1  | 2  |
//               |----|----|----|
//             1 | 3  | 4  | 5  |
//               |----|----|----|
//             2 | 6  | 7  | 8  |
//               |----|----|----|


// PIN ASSIGNMENT 
//number of rows
const int rows PROGMEM = 3;

//number of columns
const int columns PROGMEM = 3;

//number of buttons
const int buttons PROGMEM = rows * columns;

//columns and rows numbered 0, 1, 2
const int col[columns] PROGMEM = {1, 2, 3};
const int row[rows] PROGMEM = {9, 10, 11};

//arrays for button debouncing
int button_buff[buttons];
int button_lastState[buttons];
unsigned long confirm_timer[buttons]; //how long since the button reached confirmed on or off state
unsigned long debounce_timer[buttons]; //how long since the buttons state last changed
const unsigned long delayt = 20;//how long a button needs to be depressed in order to be read, in ms

//output/operation queue
LCqueue *outputBuffer; 

//cooldown timer, in case you want to prevent a button from being able to be pressed consecutively too quickly.
unsigned long cooldown_timer[buttons];

//adjust depending upon the number of buttons in the matrix
const unsigned long cooldownt[] PROGMEM = {
            0,  //0
            0,  //1
            0,  //2
            0,  //3
            0,  //4
            0,  //5
            0,  //6
            0,  //7
            0   //8
//Needs to be extended if more columns or rows are added
        };

//longhold timer, to prevent unnecessary numbers of consecutive registered button operations.  
unsigned long longhold_timer[buttons]; //timer for longhold state limits how quickly longhold triggers a registered activation 

//adjust depending upon the number of buttons in the matrix
const unsigned long longholdt[] PROGMEM = {//time to longhold state
            0,  //0
            0,  //1
            0,  //2
            0,  //3
            0,  //4
            0,  //5
            0,  //6
            0,  //7
            0   //8
//Needs to be extended if more columns or rows are added
        };

//controls how frequently the long hold operation registers an activation
const unsigned long longhold_active_t PROGMEM = 200;

//given col and row, returns button address and "number"
int buttonaddr(int rownum, int colnum){
  return (rows * rownum) + (colnum);
  }


//The media codes all require the press and release function calls in order to register
//this simply makes it easier for use in the keymap function 
void mediaFunc(int A){ //
    switch(A){
      case KEY_MEDIA_PREV_TRACK :
          Keyboard.press(KEY_MEDIA_PREV_TRACK);
          Keyboard.release(KEY_MEDIA_PREV_TRACK);
          break;
      case KEY_MEDIA_PLAY_PAUSE :
          Keyboard.press(KEY_MEDIA_PLAY_PAUSE);
          Keyboard.release(KEY_MEDIA_PLAY_PAUSE);
          break;
      case KEY_MEDIA_NEXT_TRACK :
          Keyboard.press(KEY_MEDIA_NEXT_TRACK);
          Keyboard.release(KEY_MEDIA_NEXT_TRACK);
          break;
      case KEY_MEDIA_VOLUME_DEC :
          Keyboard.press(KEY_MEDIA_VOLUME_DEC);
          Keyboard.release(KEY_MEDIA_VOLUME_DEC);
          break;
      case KEY_MEDIA_VOLUME_INC : 
          Keyboard.press(KEY_MEDIA_VOLUME_INC);
          Keyboard.release(KEY_MEDIA_VOLUME_INC);
          break;
      case KEY_MEDIA_MUTE : 
          Keyboard.press(KEY_MEDIA_MUTE);
          Keyboard.release(KEY_MEDIA_MUTE);
          break;
    }
  }

//DEFAULT CONFIGURATION
//function that handles mapping button input to desired keypad output. 
//each case corresponds to the button number assignment
void KeyMap(int A){
    switch(A){
      case 0 : // Previous Track
          mediaFunc(KEY_MEDIA_PREV_TRACK);
        break;
      case 1 : // Play/Pause
          mediaFunc(KEY_MEDIA_PLAY_PAUSE);
        break;
      case 2 : // Next Track
          mediaFunc(KEY_MEDIA_NEXT_TRACK);
        break;
      case 3 : // Volume Down
          mediaFunc(KEY_MEDIA_VOLUME_DEC);
        break;
      case 4 : // Mute
          mediaFunc(KEY_MEDIA_MUTE);
        break;
      case 5 : // Volume Up
          mediaFunc(KEY_MEDIA_VOLUME_INC);
        break;
      case 6 : //joystick 1
          Joystick.button(1, 1);      
          Joystick.button(1, 0);
        break;
      case 7 : //joystick 2
          Joystick.button(2, 1);      
          Joystick.button(2, 0);
        break;
      case 8 : //joystick 3
          Joystick.button(3, 1);      
          Joystick.button(3, 0);
        break;
      default :
      break;
      }
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
     if(button_cooldown_time >= button_confirmed_state_change_time && longholdt[address] != 0){
      //if the button has been held down long enough for a long hold mode activation
      if((millis() - button_cooldown_time) > longholdt[address]){
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
    else if((millis() - button_cooldown_time) > cooldownt[address] && button_confirmed_state_change_time > button_cooldown_time && cooldownt[address] != 0){    //if cooldown timer is exceeded, resets timer, returns true //&& button_confirmed_state_change_time > button_cooldown_time 
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
   for(cnt = 0; cnt < rows; cnt = cnt + 1){
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
    for(rownum = 0; rownum < rows; rownum = rownum + 1){   //increments rownum 
      rowSet(rownum);                                   //and sets the row with that number to HIGH.
      for(colnum = 0; colnum < columns; colnum = colnum + 1){ //increments colnum 
        debounce(rownum, colnum);                       //and runs "debounce" on that column
      }
    }
  }


//reads the current button state as determined via debounced matrix scan, 
//and then makes sure the press is registerable (has no withstanding cooldown, if it's exceeded longhold time, or if the ouput buffer is full) 
void genOutputBuffer(){
  int cnt;
  for(cnt = 0; cnt < buttons; cnt = cnt + 1){
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
    KeyMap(action);
    }
  }
  
void setup() {
  int cnt;
  //button matrix instantiation
  for (cnt = 0; cnt < rows; cnt = cnt + 1){ //instantiates every pin in row[] to OUTPUT and in col[] to INPUT_PULLUP
      pinMode(row[cnt],OUTPUT);
    }
 for (cnt = 0; cnt < columns; cnt = cnt + 1){ //instantiates every pin in row[] to OUTPUT and in col[] to INPUT_PULLUP
      pinMode(col[cnt],INPUT_PULLUP);
    }
   
  outputBuffer = new_queue(); //allocating memory for queue
}

void loop() {
  scan();
  genOutputBuffer();
  executeBuffer();
}
