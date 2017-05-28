// author: Michael Hautman
// for Arduino or Arduino library compatible devices  
#include "Arduino.h"
#include "Wire.h"
#include "LightConf.h"

void LightConf::lightWaveSin() {
	for (int cnt = 0; cnt < NumLED; cnt++) {
		lightSettings[cnt].brightness = lightSettings[cnt].range * sin(PI*(lightSettings[cnt].phase) + PI* millis() / (1000)*lightSettings[cnt].speed) + lightSettings[cnt].midInt;
	}
}

void LightConf::lightIntensityMod(int x) {
	for (int cnt = 0; cnt < 3; cnt++) {
		lightSettings[cnt].midInt += x;
	}
}

void LightConf::lightingSettingsPresets(int set) {
	switch (set) {
	case 0:

		//default
		break;
	case 1: //sin wave: move down
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].phase = 0 + (1.0 / NumLED) * ((float)cnt)/ Wavelength;
		}
		AnimationMode = 1;
		break;
	case 2: //sin wave: move up
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].phase = 1 - (1.0 / NumLED) * (cnt*1.0)/Wavelength;
		}
		AnimationMode = 1;
		break;
	case 3:
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].range = 20;
			lightSettings[cnt].midInt = 30;
		}
		AnimationMode = 1;
		break;
	case 4: //sin wave: short pulse up
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].range = 10 + 15 * cnt;
			lightSettings[cnt].midInt = 0;
			lightSettings[cnt].phase = 0 + .35 * cnt;
		}
		AnimationMode = 1;
		break;
	case 5: //reset to default
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].phase = 0;
			lightSettings[cnt].speed = 0;
			lightSettings[cnt].range = 0;
			lightSettings[cnt].midInt = 0;
		}
		AnimationMode = 0;
		break;
	case 6: //reset to default
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].phase = 0;
			lightSettings[cnt].speed = 1;
			lightSettings[cnt].range = 35;
			lightSettings[cnt].midInt = 45;
		}
		AnimationMode = 0;
		break;
	default:
		//nothing
		break;
	}
}

void LightConf::applyState() {
	for (int cnt = 0; cnt < NumLED; cnt++) {
		analogWrite(LEDPin[cnt], lightSettings[cnt].brightness);
	}
}

void LightConf::lightingFunc(int set) {
	switch (set) {
	case 0:
		//constant 

		break;
	case 1:
		lightWaveSin();

		break;
	}
}

void LightConf::setLightPreset(int x){
	GlobalLightSetting = x;
	lightingSettingsPresets(x);
	}
	
void LightConf::setWavelength(float x){
	Wavelength = x;
}	

	//constructor
LightConf::LightConf(const int *LEDPinArray, light* lightSettingsArray,const int &NumLEDin ) {
		LEDPin = LEDPinArray;
		NumLED = NumLEDin;
		lightSettings = lightSettingsArray;
	}

	//default constructor
LightConf::LightConf() {}


void LightConf::exLighting() {
		lightingFunc(AnimationMode);
		applyState();
	}
