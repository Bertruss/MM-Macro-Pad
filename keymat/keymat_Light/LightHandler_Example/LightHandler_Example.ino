#include <LightHandler.h>
//demo


const int NumLEDS = 1;//number of pins used
const int LEDpin[NumLEDS] = {3};//PWM pin numbers

light* Lights = new light[NumLEDS]; //instantiating a array of 'light' structures to be manipulated by the lighthandler object
LightHandler lightcontrol; //allocates memory for lighthandler object 'lightcontrol'

void setup() {
  //constructing instance of lighthandler
  lightcontrol = LightHandler(LEDpin, Lights, NumLEDS); 

  //sets the current animation to "blip", which does random soft flashes. 
  lightcontrol.setLightPreset(BLIP);
  
  lightcontrol.setMidIntensity(0); //sets the base brightness of all available objects. 
  lightcontrol.setRange(10); //sets tha max variance from the base brightness executed by the light animations within LightHandler
 
}

void loop() {
  lightcontrol.execute();
}

