// author: Michael Hautman
// for Arduino or Arduino library compatible devices  
#include "Arduino.h"
#include "Wire.h"
#include "LightHandler.h"

//scrubs int math for storage in uint8_t
void LightHandler::Brightnesslimit(int *x) {
	if(x < 0){
		x = 0;
	} else if(x > 255){
		x = 255;
	}
}

void LightHandler::lightWaveSin() {
	int brightness;
	for (int cnt = 0; cnt < NumLED; cnt++) {
		brightness = lightSettings[cnt].range * sin(PI*(lightSettings[cnt].phase) + PI* millis() / (1000)*lightSettings[cnt].speed) + lightSettings[cnt].midInt;
		Brightnesslimit(brightness);
		lightSettings[cnt].brightness = brightness;
	}
}

//random flashes
//follows a (-(x-1)^2 + 1) curve for growing brighter then fading.
//reaches peak brightness at blipDuration/2
void void LightHandler::blip() {
	int brightness;
	int blipDuration = 500; 
	unsigned long currentTime = millis();
	unsigned long timeSinceBlipTrue;
	for (int cnt = 0; cnt < NumLED; cnt++) {
		timeSinceBlipTrue = currentTime - lightSettings[cnt].lastblip;
		if(lightSettings[cnt].blip){
			if(timeSinceBlipTrue >=  blipDuration){
				lightSettings[cnt].blip = false;
				lightSettings[cnt].brightness = lightSettings[cnt].midInt;
			}else{
				uint8_t max = lightSettings[cnt].midInt + lightSettings[cnt].range;
				uint8_t variance = lightSettings[cnt].range - lightSettings[cnt].midInt;
				brightness = variance*(-(timeSinceBlipTrue/(blipDuration/2) - 1)^2 + 1) + lightSettings[cnt].midInt; //follows a -(x-1)^2 + 1 curve for growing brighter then fading.
				Brightnesslimit(brightness);
				lightSettings[cnt].brightness = brightness;
			}	
		}else{
			if(random(0, 1000) < 1000*(lightSettings[cnt].TTblip/(timeSinceBlip))){
				lightSettings[cnt].lastblip = currentTime;
				lightSettings[cnt].blip = true;
			}
		}
	}
}


void LightHandler::lightIntensityMod(int x) {
	for (int cnt = 0; cnt < NumLED; cnt++) {
		lightSettings[cnt].midInt += x;
		Brightnesslimit(lightSettings[cnt].brightness);
	}
}

void LightHandler::lightingSettingsPresets(int set) {
	switch (set) {
	case 0: //default
		reset();
		AnimationMode = 0;
		
		break;
	case 1: //sin wave: move up
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].phase = 0 + (1.0 / NumLED) * ((float)cnt)/ Wavelength;
		}
		AnimationMode = 1;
		break;
	case 2: //sin wave: move down
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].phase = 1 - (1.0 / NumLED) * (cnt*1.0)/Wavelength;
		}
		AnimationMode = 1;
		break;
	case 3: //sin wave: short pulse up
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].range = 10 + 15 * cnt;
			lightSettings[cnt].midInt = 0;
			lightSettings[cnt].phase = 0 + .35 * cnt;
		}
		AnimationMode = 1;
		break;
	case 4: //sin wave: short pulse up
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].range = 10 + 255/NumLED * cnt;
			lightSettings[cnt].midInt = 0;
			lightSettings[cnt].phase = 0 + .35 * cnt;
		}
		AnimationMode = 1;
		break;
	case 5: //blip

		AnimationMode = 2
		break;
	case 6: //set to 0
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].phase = 0;
			lightSettings[cnt].speed = 0;
			lightSettings[cnt].range = 0;
			lightSettings[cnt].midInt = 0;
		}
		AnimationMode = 0;
		break;
	default:
		//nothing
		break;
	}
}

void LightHandler::applyState() {
	for (int cnt = 0; cnt < NumLED; cnt++) {
		analogWrite(LEDPin[cnt], lightSettings[cnt].brightness);
	}
}

void LightHandler::lightingFunc(int set) {
	switch (set) {
	case 0:
		//constant 

		break;
	case 1:
		lightWaveSin();

		break;
	}
	case 2:
		blip();

		break;
	}
}

void LightHandler::getGlobalLightSetting(){
	return GlobalLightSetting;
}

void LightHandler::getAnimationMode(){
	return AnimationMode;
}

void LightHandler::setLightPreset(int x){
	GlobalLightSetting = x;
	lightingSettingsPresets(x);
	}
	
void LightHandler::setWavelength(float x){
	Wavelength = x;
}
	
void LightHandler::reset(){
	for (int cnt = 0; cnt < NumLED; cnt++) {
		lightSettings[cnt].phase = 0;
		lightSettings[cnt].speed = 1;
		lightSettings[cnt].range = 35;
		lightSettings[cnt].midInt = 45;
	}
	AnimationMode = 0;
}

	//constructor
LightHandler::LightHandler(const int *LEDPinArray, light* lightSettingsArray,const int &NumLEDin ) {
		unsigned long currenttime = millis();
		LEDPin = LEDPinArray;
		NumLED = NumLEDin;
		lightSettings = lightSettingsArray;
		for (int cnt = 0; cnt < NumLED; cnt++) {
			pinMode(LEDPin[cnt], OUTPUT);
			lightSettings[cnt].lastblip = currenttime;
		}
	}

	//default constructor
LightHandler::LightHandler() {}

void LightHandler::execute() {
		lightingFunc(AnimationMode);
		applyState();
	}
