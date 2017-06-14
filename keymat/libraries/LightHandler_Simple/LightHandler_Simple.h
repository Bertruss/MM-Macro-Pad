#ifndef Light_Conf
#define Light_Conf

//Preset description enum
enum lighting_presets_t { DEFAULT_SET, SIN_WAVE_FORWARD, SIN_WAVE_REV, SIN_PULSE_FORWARD, SIN_PULSE_REVERSE, BLIP, ZERO};

//animaiton mode enum
enum animation_func_t { CONST_INTENSITY, SINUSOID, RAND_BLIP};

class LightHandler_Simple{
private:
	int NumLED;
	int* LEDPin;
	lighting_presets_t GlobalLightSetting = DEFAULT_SET; //describes current preset chosen from lightingSettingsPresets
	animation_func_t AnimationMode = CONST_INTENSITY; //selects function for lighting animation 
	
	unsigned int TTblip= 10000; //Max time between Blips
	unsigned int blipDuration = 1000;
	unsigned long* lastBlip; 
	unsigned int* blipRand;
	
	float Wavelength = 1; // based upon number of light objects, e.g. a wavelength of 1 means the complete wave will be represesnted across all the lights
	bool sineMode = true; //true = normal mode, false = phaseSeparation mode
	float phaseSeparation = .35; //for "pulse" setting, determines pase separation between light objects
	float speed = 3;   // cycle rate
	
	uint8_t range = 5;  // 0-255 variance
	uint8_t midInt = 5; // 0-255 mean instensity
	
	void lightWaveSin(); //animation mode 1
	void blip(); //animation mode 2
	void lightingSettingsPresets(int set);
	void applyState();
	void lightingFunc(int set);
	void Brightnesslimit(int *x);
public:	
	LightHandler(const int *LEDPinArray, const int &NUM_LED);
	LightHandler();
	void reset();
	void getGlobalLightSetting();
	void getAnimationMode();
	void execute();
	void lightIntensityMod(int x);
	void setSineMode(bool x);
	void setPhaseSeparation(float x);
	void setSineSpeed(float x);
	void setWavelength(float x);
	void setLightPreset(int x);
	void setMidIntensity(int x);
	void setRange(int x);
	uint8_t* brightness;
};

#endif