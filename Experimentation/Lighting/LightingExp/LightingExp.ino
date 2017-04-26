const int LED1Pin =  3;
const int LED2Pin =  4;
const int LED3Pin =  6;

int lightSetting = 1;

//struct that stores 
struct lightObj{
  int brightness = 0; // 0-255 PWM number
  double phase = 0;   // shifted by pi*phase for sinusoidal functions 
  double speed = 1;   //
  int range = 20;     // variance
  int midInt = 20;    // mean instensity
  } row1Light, row2Light, row3Light;
  
lightObj lightRowSettings[3];

void lightWaveSin(){
  for(int cnt = 0;cnt < 3;cnt++){
    lightRowSettings[cnt].brightness =  lightRowSettings[cnt].range * sin(PI*(lightRowSettings[cnt].phase) + PI* millis()/(1000)*lightRowSettings[cnt].speed) +  lightRowSettings[cnt].midInt; 
  }
}

void lightIntensityMod(int x){
   for(int cnt = 0;cnt < 3;cnt++){
        lightRowSettings[cnt].midInt += x;
      }
  }

void lightFlicker(){
  for(int cnt = 0;cnt < 3;cnt++){
    lightRowSettings[cnt].brightness =  random(lightRowSettings[cnt].brightness - 1, lightRowSettings[cnt].brightness + 1);
    if(lightRowSettings[cnt].brightness > (lightRowSettings[cnt].midInt + lightRowSettings[cnt].range)
      ||lightRowSettings[cnt].brightness < (lightRowSettings[cnt].midInt - lightRowSettings[cnt].range)){
      lightRowSettings[cnt].brightness = lightRowSettings[cnt].midInt;
      }  
  }
}

void lightFlame(){
  lightFlicker();
  for( int cnt = 0;cnt < 3;cnt++){
    if(cnt != 0){
      lightRowSettings[cnt].brightness = lightRowSettings[cnt-1].brightness * .25;     
      } 
  }
}
  
void lightingConfPresets(int set){
  switch (set) {
    case 0 : //sin wave: move down
      lightRowSettings[1].phase = .5;
      lightRowSettings[2].phase = 1;     
      
      break;
    case 1 : //sin wave: move up
      lightRowSettings[1].phase = .5;
      lightRowSettings[0].phase = 1; 
      
      break;
    case 2 :
      for(int cnt = 0;cnt < 3;cnt++){
        lightRowSettings[cnt].range = 20;
        lightRowSettings[cnt].midInt = 30;
      }
      break;
     case 3 : //sin wave: short pulse up
      for(int cnt = 0;cnt < 3;cnt++){
        lightRowSettings[cnt].range = 10 + 15*cnt;
        lightRowSettings[cnt].midInt = 0;
        lightRowSettings[cnt].phase = 1 - .35 * cnt;
      }
      break;
    default :
      for(int cnt = 0;cnt < 3;cnt++){
        lightRowSettings[cnt].phase = 0;
        lightRowSettings[cnt].speed = 2;
        lightRowSettings[cnt].range = 35;
        lightRowSettings[cnt].midInt = 45;
      }
  }
}

void lightingFunc(int set){
  switch (set) {
    case 0 : 
      //constant 
    
      break;
    case 1 : 
    lightWaveSin();
      
      break;
    case 2 : 
    lightFlicker();

      break;
    case 3 : 
    lightFlame();
    
      break;
  }
  analogWrite(LED1Pin, lightRowSettings[0].brightness);
  analogWrite(LED2Pin, lightRowSettings[1].brightness);
  analogWrite(LED3Pin, lightRowSettings[2].brightness);
}  

void setup()   {        
  pinMode(LED1Pin, OUTPUT);
  pinMode(LED2Pin, OUTPUT);
  pinMode(LED3Pin, OUTPUT);
  lightRowSettings[0] = row1Light;
  lightRowSettings[1] = row2Light;
  lightRowSettings[2] = row3Light;
  lightingConfPresets(3);
}

void loop()                     
{
    lightingFunc(lightSetting);
}

