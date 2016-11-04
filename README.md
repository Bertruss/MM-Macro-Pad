# 3x3-key-matrix
Code and various design artifacts for build of a 3x3 key matrix, for use as a macro-pad. Microcontroller used is the Teensy-LC. In many way this is a very simple project, but the goal is not to break new ground so much as it is to learn new skills. Code is currently in arduino's weird format due to issues with my work environment, eventual migration to a strictly c based implementation is the intention. 

##design goals
-multiple modes
-some macros stored on device
-some possible functions:
	-single buttons for play/pause, skip, and previous
	-single button terminal opening, navigation to specific common directorys
	-single button toggling/switching between audio sources

additional:
--led lighting that is reactive to input
