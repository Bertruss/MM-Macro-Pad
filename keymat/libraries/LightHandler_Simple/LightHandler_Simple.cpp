// author: Michael Hautman
// for Arduino or Arduino library compatible devices  
#include "Arduino.h"
#include "Wire.h"
#include "LightHandler_Simple.h"

//scrubs int math for storage in uint8_t
void LightHandler_Simple::Brightnesslimit(int &x) {
	if(x < 0){
		x = 0;
	} else if(x > 255){
		x = 255;
	}
}

void LightHandler_Simple::lightWaveSin() {
	int brightness;
	float phase;
	for (int cnt = 0; cnt < NumLED; cnt++) {
		if(sinMode){
			phase = 0 + (1.0 / cnt) * ((float)cnt)/Wavelength;
		} else {
			phase = 0 + phaseSeperation * ((float)cnt)*(abs(Wavelength)/Wavelength);//wavelength is used in this case simply to preserve the direction of the wave 
		}
		brightness = range * sin(PI*(phase) + PI* millis() / (1000)*speed) + midInt;
		Brightnesslimit(brightness);
		brightness[cnt] = brightness;
	}
}

//random flashes
//follows a (-(x-1)^2 + 1) curve for growing brighter then fading.
//reaches peak brightness at blipDuration/2
void LightHandler_Simple::blip() {
	int brightness;
	unsigned long timeSinceBlipTrue;
	unsigned long currentTime = millis();
	int variance = range - midInt;
	for (int cnt = 0; cnt < NumLED; cnt++) {
		timeSinceBlipTrue = currentTime - lastblip[cnt];
		bool blip = currentTime > (blipRand[cnt] + lastBlip[cnt]);
		if(blip){
			if(timeSinceBlipTrue > (blipRand[cnt] + lastBlip[cnt] + blipDuration)){
				brightness[cnt] = midInt;
				lastBlip[cnt] = currentTime;
				blipRand[cnt] = random(0, TTblip);
			}else{
				brightness = variance*(-pow(((float)timeSinceBlipTrue/(blipDuration/2) - 1), 2) + 1) + midInt; //follows a (-(x-1)^2 + 1) curve for growing brighter then fading.
				Brightnesslimit(brightness);
				brightness[cnt] = brightness;	
			}
		}
	}
}

void LightHandler::lightIntensityMod(int x) {
	int temp = midInt + x;
	Brightnesslimit(temp);
	midInt = temp;
}

void setSineMode(bool x){
	sineMode = x;
}

void setPhaseSeparation(float x){
	phaseSeparation = x;
}
	
void setSineSpeed(float x){
	speed = x; 
}

void LightHandler:: setMidIntensity(int x) {
	Brightnesslimit(x);
	midInt = x;
}

void LightHandler::setRange(int x) {
	Brightnesslimit(x);
	range = x;
}

void LightHandler_Simple::lightingSettingsPresets(int set) {
	switch (set) {
	case DEFAULT_SET: //default
		reset();
		AnimationMode = 0;
		break;
	case SIN_WAVE_FORWARD: //sin wave: move up
		sineMode = true;
		WaveLength = 1.0;
		AnimationMode = 1;
		break;
	case SIN_WAVE_REV: //sin wave: move down
		sineMode = true;
		Wavelength = -1.0;
		AnimationMode = 1;
		break;
	case SIN_PULSE_FORWARD: //sin wave: short pulse up
		sineMode = false;
		midInt = 0;
		AnimationMode = 1;
		break;
	case SIN_PULSE_REVERSE: //sin wave: short pulse up
		sineMode = false;
		midInt = 0;
		AnimationMode = 1;
		break;
	case BLIP: //blip
		AnimationMode = 2;
		break;
	case ZERO: //set to 0
		phase = 0;
		speed = 0;
		range = 0;
		midInt = 0;
		AnimationMode = 0;
		break;
	default:
		//nothing
		break;
	}
}

void LightHandler_Simple::applyState() {
	for (int cnt = 0; cnt < NumLED; cnt++) {
		analogWrite(LEDPin[cnt], brightness[cnt]);
	}
}

void LightHandler_Simple::lightingFunc(int set) {
	switch (set) {
	case CONST_INTENSITY: //constant 
		for (int cnt = 0; cnt < NumLED; cnt++) {
			brightness = midInt;
		}

		break;
	case SINUSOID:
		lightWaveSin();

		break;
	case RAND_BLIP:
		blip();

		break;
	}
}

void LightHandler_Simple::getGlobalLightSetting(){
	return GlobalLightSetting;
}

void LightHandler_Simple::getAnimationMode(){
	return AnimationMode;
}

void LightHandler_Simple::setLightPreset(int x){
	GlobalLightSetting = x;
	lightingSettingsPresets(x);
	}
	
void LightHandler_Simple::setWavelength(float x){
	Wavelength = x;
}
	
void LightHandler_Simple::reset(){
		speed = 1;
		range = 35;
		midInt = 45;
		AnimationMode = 0;
}

	//constructor
LightHandler_Simple::LightHandler_Simple(const int *LEDPinArray, ,const int &NumLEDin ) {
		unsigned long currenttime = millis();
		LEDPin = LEDPinArray;
		NumLED = NumLEDin;
		lastBlip = new unsigned long[NumLED];
		blipRand = new unsigned int[NumLED];
		for (int cnt = 0; cnt < NumLED; cnt++) {
			pinMode(LEDPin[cnt], OUTPUT);
			lastBlip[cnt] = currenttime;
			blipRand[cnt] = random(0, TTblip);
		}
	}

	//default constructor
LightHandler_Simple::LightHandler_Simple() {}

void LightHandler_Simple::execute() {
		lightingFunc(AnimationMode);
		applyState();
	}
