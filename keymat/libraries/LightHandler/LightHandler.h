#ifndef Light_Conf
#define Light_Conf

class LightHandler{
private:
	int* LEDPin;
	int NumLED;
	int GlobalLightSetting = 0; //describes current preset chosen from lightingSettingsPresets
	int AnimationMode = 1; //selects function for lighting animation 
	float Wavelength = 1; //based upon number of light objects, e.g. a wavelength of 1 means the complete wave will be represesnted across all the lights
	void lightWaveSin(); //animation mode 1
	void blip(); //animation mode 2
	void lightingSettingsPresets(int set);
	void applyState();
	void lightingFunc(int set);
	void Brightnesslimit(int *x);
public:	
	typedef struct light light;
	LightHandler(const int *LEDPinArray, light* lightSettingsArray,const int &NUM_LED);
	LightHandler();
	void reset();
	void getGlobalLightSetting();
	void getAnimationMode();
	void exLighting();
	void setWavelength(float x);
	void setLightPreset(int x);
	void lightIntensityMod(int x);
	light* lightSettings;

};

//Preset description enum
enum lighting_presets_t { DEFAULT, SIN_WAVE_FORWARD, SIN_WAVE_REV, DIM_SUBTLE, };


//struct that stores light data
typedef struct light {
	uint8_t brightness = 10; // 0-255 PWM number
	
	bool blip = false; //"blip" state flag
	int TTblip= 5000; //"time to blip", or, the length of time at which the probability of a blip reaches %100. In milliseconds;
	unsigned long lastblip;
	
	float phase = 0;   // shifted by pi*phase for sinusoidal functions 
	float speed = 3;  
	
	uint8_t range = 5;  // 0-255 variance
	uint8_t midInt = 5; // 0-255 mean instensity
} light;

#endif