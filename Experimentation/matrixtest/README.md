#BETA README
# Extensible Button Matrix using a Teensy microcontroller

## Features

### Debounce

 - cleans input of noise that occurs from switch depressing.
 
   - there are pre-made libs for this in teensyduino, but I wrote my own code.

### Button cooldown

  - can help prevent accidental double activation of a key function by way of rapid double tap.
  
  - Independent values for each button, adjusted via the associated value in the ```cooldownt``` array
 
  - when you disable the cooldown timer, button presses are still only registered once assuming it passes debounce

### Longhold

  - you know when you hold a button for more than a second it just acts like continuous presses. that thing.
  
  - Independent values for each button, adjusted via the associated value in the ```longholdt``` array
  
  - disabled by being set to 0
  
  - is not blocked by cooldown

### Fully rebindable

  - Each key is read as their address, that address is then mapped to a case in the keymap function, which essentially
    executes as a script. For a full range of what can be put in these scripts, links to the documentation from PJRC (the teensyduino libs)
	and Arduino regarding keyboard communications and the signals that can be sent from the teensy can be found at the bottom of this document.
              
### Extensible

  - Many parts will have to be manually adjusted to account, but I have tried to make it as painless
    as possible to make your own matrix with however many rows and columns as you might want.
	Admittedly, it's still a huge hassle, but this is being worked on.

note: all time is counted in ms, or 1/1000 of a second

You can create as many rows and columns as you'd like, but this is how it is configured by default

The number on each key in the map below marks its address in the buffer as well as it's signifier in the operation queue

                  0    1    2      
               |----|----|----|
             0 | 0  | 1  | 2  |
               |----|----|----|
             1 | 3  | 4  | 5  |
               |----|----|----|
             2 | 6  | 7  | 8  |
               |----|----|----|

## Instructions For Use
In order to use this code with your teensy, you must:

1.  Have the Arduino IDE installed
2.  Have the Teensyduino libraries installed
3.  Place the LCqueue folder from "libraries" to your Arduino install's "libraries" folder
4.  Open the src file in the Arduino IDE and mouse over the Tools tab
  - make sure "Board" says whichever teensy you are using.
  - "USB Type" needs to be "KeyBoard + Mouse + Joystick"
5.  go to Sketch > Include Library > LCqueue	(this will be under "contributed libraries")
6.  At this point, the code can be compiled.
  - There are currently instructions in file for how to customize the software, which you can go through. I plan on including a more comprehensive guide soon.

##Important Links and resources
[Teensyduino](http://www.pjrc.com/teensy/td_download.html) & [Arduino IDE](https://www.arduino.cc/en/Main/Software) (May need to be a legacy release for teensyduino compatibility, check the download and install instructions on PJRC for details)

[AutoHotKey](http://ahkscript.org/) (for software macro's. It's really good, and more flexible than what can be implemented from hardware.)

Guides on PJRC for the [Keyboard](http://www.pjrc.com/teensy/td_keyboard.html), [Mouse](http://www.pjrc.com/teensy/td_mouse.html) and [Joystick](http://www.pjrc.com/teensy/td_joystick.html) libraries

[Forum thread about Media Key Codes](https://forum.pjrc.com/threads/34074-Keyboard-Media-Keys-now-%28hopefully%29-Windows-compatible)