#include <LCqueue.h>

/**
 * Types of comments
 *  (opti) - Comments that relate to optimizing code and making it more efficient
 *  (style) - Comments that relate to making your code easier to read and more
 *  professional looking.
 *
 * Miscl Notes:
 * - I will mark all my comments with 'TODO' simply because it helps me separate
 *   my comments since vim syntax-highlights the word when its in all caps in a
 *   comment.
 * - (style) Get rid of the dang witespace at the end of your lines. Its ugly and
 *   annoying to work with.
 * - (style) Try to keep you comment lines under 80 or 100 chars. Manually wrap the
 *   the lines if you need. 'set cursorline' and 'set colorcolumn=80' are nice
 *   vimrc additions that can help you stick to this.
 * - (style) You've got some icky windows end-lines in here too. (I've deleted them for
 *   my own sanity).
 * - (style hell) Dude! wtf is up with your tabbing convention (or lack there of?)
 *   in vim the key combo 'gg=G' is your friend to retab the whole file (I did).
 */


/**
 * TODO (style) - Generally you want to do block comments in this style.
 * especially for your header information. And go ahead and put it before your
 * imports.
 */
//Extensible Button Matrix using a Teensy microcontroller
//author: Michael Hautman
//year: 2016



/*
 ***********************************
 ****** CUSTOMIZABLE ELEMENTS*******
 ***********************************
 */

/*
 * NOTE:
 * Button numbering, which I refer to repeatedly as "addressing" within the comments, is very simple but important to understanding the code.
 * buttons are numbered firstly left to right then wrapping to the next row, the same order in which you would read words on a page.
 *
 * ALSO: It is mentioned elsewhere, but just to re-iterate, all time is in is ms.
 */

/**
 * TODO (opti) - <<< BEGIN SECTION COMMENT
 * These are the sorts of things that are perfect for '#define' statements
 * because they (hopefully) won't change during runtime and then there is less
 * you need to store in memory. There is a good chance that the complier is
 * optimizing these variables away for you anyway but its generally good practice
 * to do these sorts of things anyway.
 *
 * (style) Also, Defines should generally be all caps with underscores. (ie -
 * NUM_ROWS or NUM_BUTTONS). Also if you name these things NUM_ROWS instead of
 * ROWS the comments are less necessary and you can kill them to clean up you
 * code a lot.
 */
//NUMBER OF ROWS
const int rows PROGMEM = 3;

//NUMBER OF COLUMNS
const int columns PROGMEM = 3;

//number of buttons, utility
const int buttons PROGMEM = rows * columns;
/** END SECTION COMMENT >>>**/

//array for button debouncing, utility
int button_buff[buttons];

//PIN ASSIGNMENT (for the teensy)
const int col[columns] PROGMEM = {1, 2, 3};
const int row[rows] PROGMEM = {9, 10, 11};

//BUTTON COOLDOWN
//This array is where you would set the cooldown time for each button.
//This is for if there are any functions that you want to ensure can't have an accidental double activation.
//NOTE: These numbers are in ms, the associated button's identifier is commented next to each cooldown #.
/**
 * TODO (style) - 'cooldownt' looks like you typo'd a t. Maybe something like
 * 'cool_time' would help readers.
 */
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
/**
 * TODO (style) - While its obviously an integer, what is A? It looks like its
 * some sort of button code but its rather ambiguous.
 */
void KeyMap(int A){
    hardModCheck();
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
            //If you add more rows, more cases need to be added
            //    case # :
            //        YOUR CODE HERE;
            //
            //      break;

        default :
            break;
    }
    Keyboard.set_modifier(0);
}

//soft mod check is a simple function to see if a given button is depressed.
//usefull for having scripted modifiers
bool softModCheck(int I){
    int cnt;
    bool check = false;
    for(cnt = 0; cnt <  buttons; cnt = cnt + 1){
        if(button_buff[cnt] == I){
            cnt = buttons;
            check = true;
        }
    }
    return check;
}


//****CONSIDER UNIMPLEMENTED****
//Place the address of the key you wish to act as a modifier inside this list in the appropriate row.
/**
 * TODO (style) - Where does '4' come from? Might be a good idea to put this in
 * a '#define' statement near the top.
 */
const int ModKeyList[][4] = {
    {},//CTRL
    {},//SHIFT
    {},//ALT
    {}//GUI(WINDOWS or MAC button)
};


//****CONSIDER UNIMPLEMENTED****
//Hard mods are keys that implement standard modifiers like ALT and SHIFT.
//In order to use these you must include them in the ModKeyList.
void hardModCheck(){
    bool list[] = {false,false,false,false};
    int currentKeyCheck;
    int cnt;
    int mods;
    for(mods = 0; mods < 4; mods = mods + 1){
        /**
         * TODO (opti) - If you can do a calculation outside of your loop test
         * DO IT. Otherwise it is evaluated each time the loop happens.
         *
         * (style) - 'cnt++' is generally prefered over 'cnt=cnt+1' especially in
         * in loops because it helps things stay a little more clean looking.
         *
         * (style) - If your loops remain complex even after the above suggestion
         * you can put a newline after each ';' to help visually separate parts
         * of the loop
         */
        for(cnt = 0; cnt < (sizeof(ModKeyList[mods])/sizeof(int)); cnt = cnt + 1){
            currentKeyCheck = ModKeyList[cnt][mods];
            if(button_buff[currentKeyCheck] == HIGH){
                list[mods] = true;
            }
        }
    }
    //turn list into a set_modifier command
}

/**
 * TODO (style) - What this function do? Even a one line description is good.
 */
void keyPressReset(){
    Keyboard.send_now();
    Keyboard.set_modifier(0);
    Keyboard.set_key1(0);
    Keyboard.set_key2(0);
    Keyboard.set_key3(0);
    Keyboard.set_key4(0);
    Keyboard.set_key5(0);
    Keyboard.set_key6(0);
    Keyboard.send_now();
}

/*
 *************************************************
 ******* *HELPER METHODS FOR CUSTOMIZATION *******
 *************************************************
 */

//The media codes all require the press and release function calls in order to register
//this simply makes it easier for use in the keymap function.
/**
 * TODO (style) - While its obviously an integer, what is A? It looks like its
 * some sort of button code but its rather ambiguous.
 */
void mediaFunc(int A){
    /**
     * TODO (opti) - This is a pointless switchcase. It maps an enum to the same
     * enum. You could literally just do the following:
     *
     *  Keyboard.press(A);
     *  Keyboard.release(A);
     *
     * Additionally you could do this whole function with a macro as follows:
     *
     *  #define STANDARD_KEY_PRESS(a) Keyboard.press(a); Keyboard.release(a);
     *
     * This is beneficial because it is preforming a simple set of instructions
     * that you will likely repeat often. Doing it as a macro instead of a
     * function call removes the overhead of storing registers and stuff when
     * actually calling a function.
     */
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

/**
 * TODO (?style?) - I honestly have no idea what this is doing. A short comment
 * would help. Also, this could be easily MACRO-ized to remove function-call
 * overhead.
 */
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


//arrays for button debouncing
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
    else if((millis() - button_cooldown_time) > cooldownt[address] && button_confirmed_state_change_time > button_cooldown_time && cooldownt[address] != 0){    //if cooldown timer is exceeded, resets timer, returns true
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
//and then makes sure the press is registerable (has no withstanding cooldown, if it's exceeded longhold time,
//or if the output buffer has reached 100, an arbitrarily determined limit)
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
    /**
     * TODO (opti) - This is probably being overly nit-picky but I'll mention it
     * anyway. I haven't looked at your queue implementation yet so this may or
     * may not be useful.
     *
     * In C and C++ true is non-zero and false is zero. If you are just choosing
     * -1 as an error code and zero isn't a valid option its better to return 0
     * on success and a non-zero on an error. This would allow you to make a
     * decision about error-handling as follows:
     *
     *  if (!function_call(parm1, param2)) {
     *      ...
     *      <ERROR HANDLING CODE>
     *      ...
     *  }
     *
     * This also helps to optimize the program by removing an unnecessary
     * comparison.

     * Now that I've written this whole section I realize that its probably not
     * that useful in this case based on how I believe you implemented your
     * queue. If you still want to implement this you could always pass in a
     * reference to be filled so it could be called as follows:
     *
     *  int action;
     *  if (pop(outputBuffer, &action)) {
     *      KeyMap(action);
     *  }
     */
    int action = pop(outputBuffer);
    if(action != -1){
        KeyMap(action);
    }
}

void setup() {
    int cnt;
    //button matrix instantiation
    /* TODO (style) - See lines 230-240 */
    for (cnt = 0; cnt < rows; cnt = cnt + 1){ //instantiates every pin in row[] to OUTPUT and in col[] to INPUT_PULLUP
        pinMode(row[cnt],OUTPUT);
    }
    /* TODO (style) - See lines 230-240 */
    for (cnt = 0; cnt < columns; cnt = cnt + 1){ //instantiates every pin in row[] to OUTPUT and in col[] to INPUT_PULLUP
        pinMode(col[cnt],INPUT_PULLUP);
    }
    outputBuffer = new_queue(); //allocating memory for queue
}

/**
 * TODO (style) - This seems like its the main program loop. But there is nothing
 * really specifying that. Might be good to add a comment.
 */
void loop() {
    scan();
    genOutputBuffer();
    executeBuffer();
}
