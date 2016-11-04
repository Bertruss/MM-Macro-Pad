/*
experimentation/messing-around code
 */
 
//led is  pin 13
//pushbutton is pin 0
int pushbutton = 0;
int led = 13;

//runs upon intialization once pre-loop
void setup() {                
  // initialize digital pins
  pinMode(pushbutton, INPUT);
  pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever
void loop() {
  //checks pushbutton status
  if(digitalRead(pushbutton) == HIGH){//if pressed, sustains the LED for 20 ms
    digitalWrite(led, HIGH);  
    delay(20);               
 } else {                             //if not pressed, ensures LED is off for next 20 ms
    digitalWrite(led, LOW);
    delay(20);
  }                                   //after this operation is complete, loops
}
