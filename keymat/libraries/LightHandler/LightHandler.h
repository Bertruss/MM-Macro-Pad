#ifndef Light_Conf
#define Light_Conf

//Preset description enum
enum lighting_presets_t { DEFAULT_SET, SIN_WAVE_FORWARD, SIN_WAVE_REV, SIN_PULSE_FORWARD, SIN_PULSE_REVERSE, BLIP, ZERO};

//animaiton mode enum
enum animation_func_t { CONST_INTENSITY, SINUSOID, RAND_BLIP};

class LightHandler{
private:
	int NumLED;
	const int* LEDPin;
	lighting_presets_t GlobalLightSetting = DEFAULT_SET; //describes current preset chosen from lightingSettingsPresets
	animation_func_t AnimationMode = CONST_INTENSITY; //selects function for lighting animation 
	float Wavelength = 1; //based upon number of light objects, e.g. a wavelength of 1 means the complete wave will be represesnted across all the lights
	void lightWaveSin(); //animation mode 1
	void blip(); //animation mode 2
	void lightingSettingsPresets(int set);
	void applyState();
	void lightingFunc(int set);
	void Brightnesslimit(int &x);
public:	
	typedef struct light light;
	LightHandler(const int *LEDPinArray, light* lightSettingsArray,const int &NUM_LED);
	LightHandler();
	void reset();
	lighting_presets_t getGlobalLightSetting();
	animation_func_t getAnimationMode();
	void execute();
	void lightIntensityMod(int x);
	void setWavelength(float x);
	void setLightPreset(int x);
	void setMidIntensity(int x);
	void setRandomInterval(unsigned int x);
	void setRange(int x);
	light* lightSettings;
};

//struct that stores light data
typedef struct light {
	uint8_t brightness = 0; // 0-255 PWM number
	
	bool blip = false; //"blip" state flag
	unsigned int TTblip= 5000; //"time to blip", or, the length of time at which the probability of a blip reaches %100. In milliseconds;
	unsigned long lastblip = 0;
	unsigned int blipDuration = 1000;
	int blipRand;
	
	float phase = 0;   // shifted by pi*phase for sinusoidal functions
	float speed = 1;   // cycle rate
	
	uint8_t range = 5;  // 0-255 variance
	uint8_t midInt = 5; // 0-255 mean instensity
} light;

#endif