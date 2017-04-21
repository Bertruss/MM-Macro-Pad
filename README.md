# Key-matrix
Code and various design artifacts for build of a key matrix, for use as a macro-pad. Microcontroller used is the *Teensy-LC*. In many ways this is a very simple project, but the goal is not to break new ground so much as it is to learn new skills. Code is currently using the teensyduino libraries for ease of implementation, but I'm hopeful to eventually try the more "hands on" approach, with less reliance on the arduino ide and teensyduino libraries.

## Current features
- option for button cooldown
- option for longhold function 
  - e.g. a held button repeatedly executes the buttons associated function
- supports button assignments for: 
  - media key functions
  - function keys  
  - arrow keys
  - "joystick" buttons (convenient for software macros)
  - all typical keyboard key assignments
- support for scripting
- can be adjusted to any number of rows or columns

## additional design goals (yet to be implemented)
- multiple modes for macros and functions.
- modifier keys for multi-key macros.
- led lighting that is reactive to input

## Hardware
- Hardware folder contains design files for the layout of the keypad.
	- layout for the faceplate
	- rough parts manifest, with available sources and cost estimation


## Instructions for compilation & custom configuration as well as
## release source can be found in the Keymat folder. 
