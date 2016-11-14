//9 button matrix with 7-seg output
//prints which button has been pressed to 7-seg display

// pin assignment
const int col[3] PROGMEM = {1, 2, 3};
const int row[3] PROGMEM = {9, 10, 11};

int button_buff[9];
int button_lastState[9];
unsigned long debounce_timer[9];


int buttonaddr(int rownum, int colnum){
  return (3 * rownum) + (colnum + 1);
  }

//debouncing pin reader
void debounce(int rownum, int colnum){
  int address = buttonaddr(rownum, colnum);
  int pin = col[colnum];
  unsigned long delayt = 1000;//how long a button needs to be depressed in order to be read
  int readPin = digitalRead(pin);
  
  if(readPin != button_lastState[address]){
    debounce_timer[address] = millis();
    }
    
  if((millis() - debounce_timer[address]) > delayt && readPin != button_buff[address]){
    button_buff[address] = readPin;
    }
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
        if(button_buff[cnt]){
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

}


