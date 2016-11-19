# 3x3-key-matrix
Code and various design artifacts for build of a 3x3 key matrix, for use as a macro-pad. Microcontroller used is the Teensy-LC. In many ways this is a very simple project, but the goal is not to break new ground so much as it is to learn new skills. Code is currently using the teensyduino libraries for ease of implementation, but I'm hopeful to eventually try the more "hands on" approach, with less reliance on the arduino ide and teensyduino libraries.

##Current attributes
- debounce
- option for button cooldown
- option for longhold function 
  - (e.g. a button activation longer than a second causes the button to rapidly execute it's associated function) 

##Design goals (yet to me implemented)
- multiple modes for macros and functions
- some possible functions:
  - single buttons for play/pause, skip, and previous
  - single button terminal opening, navigation to specific common directorys
  - single button toggling/switching between audio sources

additional:
  - led lighting that is reactive to input


##Instructions for compilation and release source can be found in the Keymat folder. 
