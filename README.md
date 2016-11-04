# 3x3-key-matrix
Code and various design artifacts for build of a 3x3 key matrix, for use as a macro-pad. Microcontroller used is the Teensy-LC. In many ways this is a very simple project, but the goal is not to break new ground so much as it is to learn new skills. Code is currently using the teensyduino libraries for ease of implementation, but I'm hopeful to eventually try the more "hands on" approach, with less reliance on the arduino ide and teensyduino libraries.

##design goals
- multiple modes
- some macros stored on device
- some possible functions:
  - single buttons for play/pause, skip, and previous
  - single button terminal opening, navigation to specific common directorys
  - single button toggling/switching between audio sources

additional:
  - led lighting that is reactive to input
