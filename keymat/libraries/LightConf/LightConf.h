#ifndef Light_Conf
#define Light_Conf

class LightConf{
private:
	int* LEDPin;
	int NumLED;
	int GlobalLightSetting = 0; //describes current preset chosen from lightingSettingsPresets
	int AnimationMode = 1; //selects function for lighting animation 
	float Wavelength = 1; //based upon number of rows, columns, or lights, depending upon how you want it to work
	void lightWaveSin(); //animation
	void lightingSettingsPresets(int set);
	void applyState();
	void lightingFunc(int set);
public:	
	typedef struct light light;
	LightConf(const int *LEDPinArray, light* lightSettingsArray,const int &NUM_LED);
	LightConf();
	void exLighting();
	void setWavelength(float x);
	void setLightPreset(int x);
	void lightIntensityMod(int x);
	light* lightSettings;

};

//struct that stores light data
typedef struct light {
	uint8_t brightness = 10; // 0-255 PWM number
	float phase = 0;   // shifted by pi*phase for sinusoidal functions 
	float speed = 3;  
	uint8_t range = 5;  // 0-255 variance
	int midInt = 5; // 0-255 mean instensity
} light;

#endif