//9 button matrix with cooldown and debounce

// pin assignment
const int col[3] PROGMEM = {1, 2, 3};
const int row[3] PROGMEM = {9, 10, 11};

//arrays for button debouncing and cooldown
int button_buff[9];
int button_lastState[9] = {0,0,0,0,0,0,0,0,0};
unsigned long debounce_timer[9];
unsigned long cooldown_timer[9];

//how long a button needs to be depressed in order to be read
const unsigned long delayt = 50;
const unsigned cooldown = 300;

int buttonaddr(int rownum, int colnum){
  return (3 * rownum) + (colnum + 1);
  }

//debouncing pin reader
void debounce(int rownum, int colnum){
  int address = buttonaddr(rownum, colnum);
  int readPin = digitalRead(col[colnum]);
  
  if(readPin != button_lastState[address]){
    debounce_timer[address] = millis();
    }
    
  if((millis() - debounce_timer[address]) > delayt && readPin != button_buff[address]){
      //if its been more than the cooldown timer since last transitioned from high to low, allow button to be set to high 
      if(((millis() - cooldown_timer[address]) > cooldown && readPin == HIGH) || readPin == LOW){
        button_buff[address] = readPin;
      } 
      
      //when going from high to low, reset cooldown marker;
      if(readPin == LOW){ 
        cooldown_timer[address] = millis();
      }
    }
  button_lastState[address] = readPin;
  }

  
//sets chosen row high and all others low
void rowSet(int rowIn) { 
   int cnt;
   for(cnt = 0; cnt < 3; cnt = cnt + 1){
     if(cnt == rowIn){
      digitalWrite(row[cnt], HIGH);
      }
     else{
      digitalWrite(row[cnt], LOW);
      }
     }
   }


//checks every button in the matrix, runs debounce on it 
void scan(){
    //numbered by array index 0 to two. 
    int rownum;
    int colnum;
    //reads every key, stores in the button buffer array.
    for(rownum = 0; rownum < 3; rownum = rownum + 1){//checks each row
      rowSet(rownum);
      for(colnum = 0; colnum < 3; colnum = colnum + 1){
        debounce(rownum, colnum);
      }
    }
  }

void bufferRead(){ //only reads one key at a time, but this is just debug
    int cnt;
    for(cnt = 0; cnt < 8; cnt = cnt + 1){
        if(button_buff[cnt] == HIGH){
            int out = cnt + 1;
            Keyboard.print(out);
          }    
      }
   }

void setup() {
  int cnt;
    //button matrix instantiation
  for (cnt = 0; cnt < 3; cnt = cnt + 1){ //instantiates every pin in row[] to OUTPUT and in col[] to INPUT_PULLUP
      pinMode(row[cnt],OUTPUT);
      pinMode(col[cnt],INPUT_PULLUP);
    }
}

void loop() {
  scan();
  bufferRead();
}


