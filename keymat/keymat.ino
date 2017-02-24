/* Extensible Button Matrix using a Teensy microcontroller
   author: Michael Hautman
   year: 2016
*/

#include <LCqueue.h>

// `a` is the code for the key press you wish to pass.
#define STANDARD_KEY_PRESS(a) Keyboard.press(a); Keyboard.release(a);

// `a` is the number of the joystick button you wish to transmit a
// keypress of.
#define JOYSTICK_BUTTON_PRESS(a) Joystick.button(a, 1);

/*
 ***********************************
 ****** CUSTOMIZABLE ELEMENTS*******
 ***********************************
*/

/*
   NOTE:
   Button numbering, which is referred to repeatedly as "address" within the comments, is very simple 
   but important to understanding the code. buttons are numbered firstly left to right then wrapping 
   to the next row, the same order in which you would read words on a page. It is mentioned elsewhere, 
   but just to re-iterate, all time is in is ms.
*/

// This setting controls the number of rows the teensy expects
#define NUM_ROWS 3

// This setting controls the number of columns the teensy expects
#define NUM_COLUMNS 3

// Number of buttons
#define BUTTONS (NUM_ROWS * NUM_COLUMNS)

// Array for button debouncing
int button_buff[BUTTONS];

// Pin Assignment for the columns and rows of the key matrix 
// You must assign as many pins as there are columns and rows
const int col[NUM_COLUMNS] PROGMEM = {1, 2, 3};
const int row[NUM_ROWS] PROGMEM = {9, 10, 11};
const int modeKey PROGMEM = 12;

/* BUTTON COOLDOWN
   This array is where you would set the cooldown time for each button.
   This is useful if there are any functions that you want to ensure can't have an accidental double activation.
   NOTE: These numbers are in ms, the associated button's identifier is commented next to each cooldown #.
*/
const unsigned long cooldown_time[] PROGMEM = {
  0,  // 0
  0,  // 1
  0,  // 2
  0,  // 3
  0,  // 4
  0,  // 5
  0,  // 6
  0,  // 7
  0   // 8
  // Needs to be extended to the correct number of buttons if more columns or rows are added
};

/* LONGHOLD
   This is where you set the length of time to "longhold state" for each key, where if you've been holding down a button for
   longer than the set longhold time for that button, the button's associated operation runs repeatedly. This mirrors what you
   expect a normal keyboard to do with a button for a single ascii character. I wouldn't recommend having this enabled for anything
   particularly complicated.

   NOTE: These numbers are in ms, the associated button's identifier is commented next to each longhold #.

   NOTE #2: Longhold is conditionally blocked by cooldown. e.g. a button with a cooldown of 2 seconds and a longhold
   of 1 second will still reach longhold state in 1 second. cooldown only applies to buttons that have been
   pressed and then released. In the case where a button *has* been pressed and then released, the time to 
   longhold state for the next button press will be measured from the first registered activation, 
   e.g. after cooldown has finished. This was an arbitrary decision, but mixing these two features generally 
   yields dumb results anyway. 
*/

const unsigned long longholdt[] PROGMEM = {
  0,  // 0
  0,  // 1
  0,  // 2
  0,  // 3
  0,  // 4
  0,  // 5
  0,  // 6
  0,  // 7
  0   // 8
  //Needs to be extended to the correct number of keys if more columns or rows are added
};

const unsigned long longhold_active_t PROGMEM = 200;//controls how frequently the long hold operation registers an activation

/* BUTTON MACRO SETTINGS   -[DEFAULT CONFIGURATION]-
   This function handles mapping button input to desired keypad output.
   The keypad supports basic scripting, "binds", media key functionality,
   as well as "blank" joystick buttons which can be easily mapped into more
   complex scripts through software such as AutoHotKey (which I highly recommend).
   I have tried to make the media Key functionality cleaner to implement with the
   "STANDARD_KEY_PRESS" macro, which sends a single press and release. 

   Each case # corresponds to the button's identifier in the matrix, and the script or what-have-you that you'd like to
   have execute goes after the desired "case # :" but before the "break;".
*/

void KeyMap(int address) {
  // hardModCheck();
  switch (address) {
    case 0 : // Previous Track
      STANDARD_KEY_PRESS(KEY_MEDIA_PREV_TRACK);

      break;
    case 1 : // Play/Pause
      STANDARD_KEY_PRESS(KEY_MEDIA_PLAY_PAUSE);

      break;
    case 2 : // Next Track
      STANDARD_KEY_PRESS(KEY_MEDIA_NEXT_TRACK);

      break;
    case 3 : // Volume Down
      STANDARD_KEY_PRESS(KEY_MEDIA_VOLUME_DEC);

      break;
    case 4 : // Mute
      STANDARD_KEY_PRESS(KEY_MEDIA_MUTE);

      break;
    case 5 : // Volume Up
      STANDARD_KEY_PRESS(KEY_MEDIA_VOLUME_INC);

      break;
    case 6 : //joystick 1
      JOYSTICK_BUTTON_PRESS(1);

      break;
    case 7 : //joystick 2
      JOYSTICK_BUTTON_PRESS(2);

      break;
    case 8 : //joystick 3
      JOYSTICK_BUTTON_PRESS(3);

      break;
      
    // If you add more rows or columns, more cases need to be added
    
    // *script stereotype*
    /*    
           case # :
              YOUR CODE HERE;
    
            break;
    */
    
    default :
      break;
  }

}

// Soft Mod check is a simple function to see if a given button is depressed.
// Usefull for having scripted modifiers
bool softModCheck(int I) {
  int address;
  bool isPressedCheck = false;
  for (address = 0; address <  BUTTONS; address++) {
    if (button_buff[address] == I) {
      address = BUTTONS;
      isPressedCheck = true;
    }
  }
  return isPressedCheck;
}

/* WIP
  // Place the address of the key you wish to act as a modifier inside this list in the appropriate row.
  // If you wish to have more than two of any particular mod key the second dimension of the array 
  // must be edited.-----v 
  const int ModKeyList[][2] = {
    {},// CTRL
    {},// SHIFT
    {},// ALT
    {}// GUI(WINDOWS or MAC button)
  };
  
    // Hard mods are keys that implement standard modifiers like ALT and SHIFT.
    // In order to use these you must include them in the ModKeyList.
    void hardModCheck() {
      bool isPressedList[] = {false, false, false, false};
      int currentKeyCheck = 0;
      int cnt = 0;
      int mods = 0;
      int num;//number of modifiers of a certain type
      for(mods = 0; mods < 4; mods++) {
        num = (sizeof(ModKeyList[mods]) / sizeof(int));
        if (num != 0) {
          for (cnt = 0; cnt < num; cnt++) {
            currentKeyCheck = ModKeyList[cnt][mods];
            if (button_buff[currentKeyCheck] == HIGH) {
              isPressedList[mods] = true;
            }
          }
        }
      }
      //turn confirmed key presses into sent key presses here.
      if (isPressedList[3]) {
        Keyboard.set_modifier(MODIFIERKEY_GUI);
      }
      if (isPressedList[0]) {
        if (isPressedList[1] && !isPressedList[2]) {
          Keyboard.set_modifier(MODIFIERKEY_CTRL | MODIFIERKEY_SHIFT);
        }
        else if (isPressedList[2] && !isPressedList[1]) {
          Keyboard.set_modifier(MODIFIERKEY_CTRL | MODIFIERKEY_ALT);
        }
        else {
          Keyboard.set_modifier(MODIFIERKEY_CTRL);
        }
      } else if (isPressedList[1]) {
        Keyboard.set_modifier(MODIFIERKEY_SHIFT);
      } else if (isPressedList[2]) {
        Keyboard.set_modifier(MODIFIERKEY_ALT);
      }
    }
*/

/*
 ******************************************
 ****** NON - CUSTOMIZABLE ELEMENTS *******
 ******************************************
*/

// Arrays for button debouncing
int button_lastState[BUTTONS];
unsigned long confirm_timer[BUTTONS];  // Last moment in time when the state of the button was confirmed via debounce
unsigned long debounce_timer[BUTTONS]; // Last moment in time when the state of the button was directly read to have changed
const unsigned long delayt = 10;       // How long a button needs to be depressed in order to be confirmed via debounce, in ms

// Output/operation queue
LCqueue *outputBuffer;

// Cooldown time marker array
unsigned long cooldown_timer[BUTTONS];

// Longhold time marker array
unsigned long longhold_timer[BUTTONS];

// Given col and row, returns button address
int buttonaddr(int rownum, int colnum) {
  return (NUM_ROWS * rownum) + (colnum);
}

// Debouncing pin reader, prevents rapid on/off switching or registering multiple button presses due to switch noise.
void debounce(int rownum, int colnum) {
  int address = buttonaddr(rownum, colnum);

  // Current state of pin assigned to pinRead
  int readPin = digitalRead(col[colnum]);

  // If pin state changes, reset debounce time marker to current time
  if (readPin != button_lastState[address]) {
    debounce_timer[address] = millis();
  }

  // If pin has not changed state for longer than the debounce delay (delayt), then commit current state to button_buff
  if ((millis() - debounce_timer[address]) > delayt && readPin != button_buff[address]) {
    confirm_timer[address] = millis();
    button_buff[address] = readPin;
  }
  button_lastState[address] = readPin;
}

// Checks whether the button at the given address has reached longhold state
bool checkLongHold(unsigned long button_cooldown_time,  unsigned long button_confirmed_state_change_time, int address) {
  /*
     ">=" is used because it happens on a shorter than ms scale. logically, if they're equal, cooldown reset had to happen
      after the state change reset because of the order of the flow. This next part is broken up to be
      easier to explain and read.
  */
  if (button_cooldown_time >= button_confirmed_state_change_time // If the last time buttonstate changed was before the last time
      && longholdt[address] != 0) {                              // the cooldown timer was reset, and the address has a longhold value set,
                                                                 // continues.
    if ((millis() - button_cooldown_time) > longholdt[address]) { // If the button has been held down long enough for a long hold mode activation

      if ((millis() - longhold_timer[address]) > longhold_active_t) { // This is another timer checker used to limit how fast 
                                                                      // the longhold state registers activations
        longhold_timer[address] = millis();
        return true;
      }
    }
  }
  return false;
}


// Multistage checker that handles cooldown and longhold functionality logic
bool checkExceptionTimers(int address) { // Returns true if cooldown timer is exceeded, or if a longhold state activation is reached
  unsigned long button_cooldown_time = cooldown_timer[address];
  unsigned long button_confirmed_state_change_time = confirm_timer[address];
  if (checkLongHold(button_cooldown_time, button_confirmed_state_change_time, address)) {
    return true;
  }
  else if (cooldown_time[address] != 0                                       // If the button has a cooldown timer set
           && button_confirmed_state_change_time > button_cooldown_time      // and the last time the buttons confirmed state change is sooner
           && (millis() - button_cooldown_time) > cooldown_time[address] ) { // than the last time button cooldown was reset and cooldown timer is exceeded
    cooldown_timer[address] = millis();                                      // resets timer, returns true
    return true;
  }
  else if (button_confirmed_state_change_time > button_cooldown_time) { // Makes sure that if the cooldown is disabled, one button
    cooldown_timer[address] = millis();                                 // press still only registers once unless longhold is exceeded
    return true;
  }
  else {
    return false; // All else return false.
  }
}

// Sets chosen row high and all others low
void rowSet(int rowIn) {
  int cnt;
  for (cnt = 0; cnt < NUM_ROWS; cnt = cnt + 1) {
    if (cnt == rowIn) {
      digitalWrite(row[cnt], HIGH);
    }
    else {
      digitalWrite(row[cnt], LOW);
    }
  }
}


// Checks every button in the matrix, runs "debounce" on it
void scan() {
  //numbered by array index 0 to 2.
  int rownum, colnum;
  for (rownum = 0; rownum < NUM_ROWS; rownum = rownum + 1) {      // Increments rownum
    rowSet(rownum);                                               // and sets the row with that number to HIGH.
    for (colnum = 0; colnum < NUM_COLUMNS; colnum = colnum + 1) { // Increments colnum
      debounce(rownum, colnum);                                   // and runs "debounce" on that column
    }
  }
}

/* Reads the current button state as determined via debounced button matrix scan,
   and then makes sure the press is registerable (has no withstanding cooldown,
   if it's exceeded longhold time, or if the output buffer has reached 100, an arbitrarily determined limit).
*/
void genOutputBuffer() {
  int address;
  for (address = 0; address < BUTTONS; address++) {
    int state = button_buff[address];
    if (state == HIGH && checkExceptionTimers(address) && (count(outputBuffer) < 100)) {
      push(outputBuffer, address);
    }
  }
}

// Executes operations described by the output.
void executeBuffer() {
  if (!isEmpty(outputBuffer)) { // If the queue is empty, does nothing
    KeyMap(pop(outputBuffer));
  }
}

// Runs once before loop()
void setup() {
  int cnt;
  //button matrix instantiation
  for (cnt = 0; cnt < NUM_ROWS; cnt++) {    // Instantiates every pin in row[] to OUTPUT
    pinMode(row[cnt], OUTPUT);
  }
  for (cnt = 0; cnt < NUM_COLUMNS; cnt++) { // Instantiates every pin in col[] to INPUT_PULLUP
    pinMode(col[cnt], INPUT_PULLUP);
  }
  outputBuffer = new_queue();               // Allocating memory for queue
}

// Main Loop, which runs continuously
void loop() {
  scan();
  genOutputBuffer();
  executeBuffer();
}
