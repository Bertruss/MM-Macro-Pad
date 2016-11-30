#include <LCqueue.h>

//Extensible Button Matrix using a Teensy microcontroller

//author: Michael Hautman
//year: 2016

/*
 ***********************************
 ****** CUSTOMIZABLE ELEMENTS*******
 ***********************************
 */

//NUMBER OF ROWS
const int rows PROGMEM = 3;

//NUMBER OF COLUMNS
const int columns PROGMEM = 3;

//PIN ASSIGNMENT (for the teensy)
const int col[columns] PROGMEM = {1, 2, 3};
const int row[rows] PROGMEM = {9, 10, 11};

//BUTTON COOLDOWN
//This array is where you would set the cooldown time for each button.
//This is for if there are any functions that you want to ensure can't have an accidental double activation.
//NOTE: These numbers are in ms, the associated button's identifier is commented next to each cooldown #.
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
            //Needs to be extended to the correct number of buttons if more columns or rows are added
        };

//LONGHOLD 
// this is where you set the time to "longhold state", where if you've been holding down a button for 
// longer than the set longhold time for that button, the button's associated operation runs repeatedly. 
// This mirrors what you expect a normal keyboard to do with a button for a single ascii character. I wouldn't recommend 
// having this enabled for anything particularly complicated. 
// NOTE: These numbers are in ms, the associated button's identifier is commented next to each longhold #.
// 2nd NOTE: Longhold is not blocked by cooldown. e.g. a button with a cooldown of 2 seconds and a longhold 
// of 1 second will still reach longhold state in 1 second. cooldown only applies to buttons that have been 
// pressed and then released. 
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
            //Needs to be extended to the correct number of buttons if more columns or rows are added
        };

const unsigned long longhold_active_t PROGMEM = 200;//controls how frequently the long hold operation registers an activation


//BUTTON MACRO SETTINGS   -[DEFAULT CONFIGURATION]-
// This function handles mapping button input to desired keypad output.
// The keypad supports basic scripting, "binds", media key functionality, 
// as well as "blank" joystick buttons which can be easily mapped into more 
// complex macro's through software such as AutoHotKey (which I highly recommend). 
// I have tried to make the media Key functionality cleaner to implement with the 
// "mediaFunc" function, and joystick button's with the "joybutton", for single 
// presses and releases of those buttons. 
// 
// Each case # corresponds to the button's identifier in the matrix, and the script or what-have-you that you'd like to
// have execute goes after the desired "case # :" but before the "break;".
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
          joyButton(1);
          
        break;
      case 7 : //joystick 2
          joyButton(2);
          
        break;
      case 8 : //joystick 3
          joyButton(3);
          
        break;
      default :
      break;
      }
  }

//The media codes all require the press and release function calls in order to register
//this simply makes it easier for use in the keymap function. 
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

void joyButton(int assign){//joystick buttons can be assigned from 1-32
   Joystick.button(assign, 1);      
   Joystick.button(assign, 0);
  }


/*
 ******************************************
 ****** NON - CUSTOMIZABLE ELEMENTS *******
 ******************************************
 */

//Marginally more complicated stuff down here. I didn't really plan for these next parts to be modified for individual use cases.  
//Not that I guarantee anything, but I extra don't guarantee anything if you mess with the proceeding code.
  
//number of buttons
const int buttons PROGMEM = rows * columns;

//arrays for button debouncing
int button_buff[buttons];
int button_lastState[buttons];
unsigned long confirm_timer[buttons]; //how long since the button reached confirmed on or off state
unsigned long debounce_timer[buttons]; //how long since the buttons state last changed
const unsigned long delayt = 10;//how long a button needs to be depressed in order to be read, in ms

// output/operation queue
LCqueue *outputBuffer; 

// cooldown timer, in case you want to prevent a button from being able to be pressed consecutively too quickly.
unsigned long cooldown_timer[buttons];

//longhold timer, to prevent unnecessary numbers of consecutive registered button operations.  
unsigned long longhold_timer[buttons]; //timer for longhold state limits how quickly longhold triggers a registered activation 

//given col and row, returns button address and "number"
int buttonaddr(int rownum, int colnum){
  return (rows * rownum) + (colnum);
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
     /* 
      * ">=" is used because it happens on a shorter than ms scale. logically, if they're equal, cooldown reset had to happen 
      *  after the state change reset because of the order of the flow. this next part is broken up to be 
      *  easier to explain and read.   
      */
     if(button_cooldown_time >= button_confirmed_state_change_time && longholdt[address] != 0){//if the last time buttonstate changed was before the last time the cooldown timer was reset, continues. 
      
      if((millis() - button_cooldown_time) > longholdt[address]){//if the button has been held down long enough for a long hold mode activation
          
          if((millis() - longhold_timer[address]) > longhold_active_t){//this is another timer checker used to limit how fast the longhold state registers activations
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


//reads the current button state as determined via debounced button matrix scan, 
//and then makes sure the press is registerable (has no withstanding cooldown, if it's exceeded longhold time, or if the output buffer has reached 100, an arbitrarily determined limit) 
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
