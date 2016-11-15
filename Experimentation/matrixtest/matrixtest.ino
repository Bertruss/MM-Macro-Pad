//9 button matrix with cooldown and debounce

// pin assignment
//columns and rows numbered 0, 1, 2
const int col[3] PROGMEM = {1, 2, 3};
const int row[3] PROGMEM = {9, 10, 11};

// You can map the keys however you wish, 
// but this is how it is  configured by default
//
//       the number on each key in the map
//       below marks its index in the buffer
//       as well as it's signifier in the
//       operation queue
//
//               0    1    2      
//            |----|----|----|
//          0 | 0  | 1  | 2  |
//            |----|----|----|
//          1 | 3  | 4  | 5  |
//            |----|----|----|
//          2 | 6  | 7  | 8  |
//            |----|----|----|


//arrays for button debouncing and cooldown
int button_buff[9];
int button_lastState[9] = {0,0,0,0,0,0,0,0,0};
unsigned long debounce_timer[9];

//how long a button needs to be depressed in order to be read, in ms
const unsigned long delayt = 20;

//given col and row, returns button address
int buttonaddr(int rownum, int colnum){
  return (3 * rownum) + (colnum);
  }

//debouncing pin reader
void debounce(int rownum, int colnum){
  int address = buttonaddr(rownum, colnum);
  int readPin = digitalRead(col[colnum]);
  
  if(readPin != button_lastState[address]){
    debounce_timer[address] = millis();
    }
    
  if((millis() - debounce_timer[address]) > delayt && readPin != button_buff[address]){
      button_buff[address] = readPin;
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


//checks every button in the matrix, runs "debounce" on it 
void scan(){
    //numbered by array index 0 to two. 
    int rownum, colnum;
    for(rownum = 0; rownum < 3; rownum = rownum + 1){//increments rownum and sets the row with that number to HIGH
      rowSet(rownum);
      for(colnum = 0; colnum < 3; colnum = colnum + 1){//increments colnum and runs "debounce" on that column
        debounce(rownum, colnum);
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
}


