// author: Michael Hautman
// for Arduino or Arduino library compatible devices  
#include "Arduino.h"
#include "Wire.h"
#include "LightHandler.h"

//scrubs int math for storage in uint8_t
void LightHandler::Brightnesslimit(int &x) {
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
void LightHandler::blip() {
	int brightness;
	unsigned long currentTime = millis();
	unsigned long timeSinceBlipTrue;
	for (int cnt = 0; cnt < NumLED; cnt++) {
		timeSinceBlipTrue = currentTime - lightSettings[cnt].lastblip;
		if(lightSettings[cnt].blip){
			if(timeSinceBlipTrue >=  lightSettings[cnt].blipDuration){
				lightSettings[cnt].blip = false;
				lightSettings[cnt].brightness = lightSettings[cnt].midInt;
			}else{
				int variance = lightSettings[cnt].range - lightSettings[cnt].midInt;
				variance = abs(variance);
				brightness = variance*(-pow(((float)timeSinceBlipTrue/(lightSettings[cnt].blipDuration/2) - 1), 2) + 1) + lightSettings[cnt].midInt; //follows a -(x-1)^2 + 1 curve for growing brighter then fading.
				Brightnesslimit(brightness);
				lightSettings[cnt].brightness = brightness;
			}	
		}else{
			int ratio = 1000*((float)timeSinceBlipTrue/lightSettings[cnt].TTblip);
			if(lightSettings[cnt].blipRand < ratio){
				lightSettings[cnt].lastblip = currentTime;
				lightSettings[cnt].blip = true;
				lightSettings[cnt].blipRand = random(0, 1000);
			}
		}
	}
}

void LightHandler::lightIntensityMod(int x) {
	int temp = (int)lightSettings[0].midInt + x;
	Brightnesslimit(temp);
	for (int cnt = 0; cnt < NumLED; cnt++) {	
		lightSettings[cnt].midInt = temp;
	}
}

void LightHandler:: setMidIntensity(int x) {
	Brightnesslimit(x);
	for (int cnt = 0; cnt < NumLED; cnt++) {
		lightSettings[cnt].midInt = x;
	}
}

void LightHandler::setRange(int x) {
	Brightnesslimit(x);
	for (int cnt = 0; cnt < NumLED; cnt++) {
		lightSettings[cnt].range = x;
	}
}

void LightHandler::lightingSettingsPresets(int set) {
	switch (set) {
	case DEFAULT_SET: //default
		reset();
		AnimationMode = CONST_INTENSITY;
		
		break;
	case SIN_WAVE_FORWARD: //sin wave: move up
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].phase = 0 + (1.0 / NumLED) * ((float)cnt)/ Wavelength;
		}
		AnimationMode = SINUSOID;
		break;
	case SIN_WAVE_REV: //sin wave: move down
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].phase = 1 - (1.0 / NumLED) * (cnt*1.0)/Wavelength;
		}
		AnimationMode = SINUSOID;
		break;
	case SIN_PULSE_FORWARD: //sin wave: short pulse up
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].range = 10 + 245 - 245/NumLED * cnt;
			lightSettings[cnt].midInt = 0;
			lightSettings[cnt].phase = 1 - .35 * cnt;
		}
		AnimationMode = SINUSOID;
		break;
	case SIN_PULSE_REVERSE: //sin wave: short pulse up
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].range = 10 + 245/NumLED * cnt;
			lightSettings[cnt].midInt = 0;
			lightSettings[cnt].phase = 0 + .35 * cnt;
		}
		AnimationMode = SINUSOID;
		break;
	case BLIP: //blip
		
		AnimationMode = RAND_BLIP;
		break;
	case ZERO: //set to 0
		for (int cnt = 0; cnt < NumLED; cnt++) {
			lightSettings[cnt].phase = 0;
			lightSettings[cnt].speed = 0;
			lightSettings[cnt].range = 0;
			lightSettings[cnt].midInt = 0;
		}
		AnimationMode = CONST_INTENSITY;
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
	case CONST_INTENSITY:
		for (int cnt = 0; cnt < NumLED; cnt++) {
		lightSettings[cnt].brightness = lightSettings[cnt].midInt;
	}
		//constant 

		break;
	case SINUSOID:
		lightWaveSin();

		break;
	case RAND_BLIP:
		blip();

		break;
	}
}

lighting_presets_t LightHandler::getGlobalLightSetting(){
	return GlobalLightSetting;
}

animation_func_t LightHandler::getAnimationMode(){
	return AnimationMode;
}


void LightHandler::setRandomInterval(unsigned int x){
	
}

void LightHandler::setLightPreset(int x){
	GlobalLightSetting = (lighting_presets_t)x;
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
	AnimationMode = CONST_INTENSITY;
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
			lightSettings[cnt].blipRand = random(0, 1000);
		}
	}

	//default constructor
LightHandler::LightHandler() {}

void LightHandler::execute() {
		lightingFunc(AnimationMode);
		applyState();
	}
