//V1.0
//9 button matrix with 7-seg output
//prints which button has been pressed to 7-seg display

// pin assignment
const int col[3] PROGMEM = {1, 2, 3};
const int row[3] PROGMEM = {9, 10, 11};

//consider this a beta implementation. 
//Working on just having a list of integer representations of the keys
bool button_buff[9] = {false,false,false,false,false,false,false,false,false};



void colRead(int row){ //handles column polling, rows numbered from 0 to 2
  int cnt;
  int rowNum = row*3;
  for(cnt = rowNum; cnt < 3; cnt = cnt + 1){
     if(digitalRead(col[cnt])== HIGH){ 
      button_buff[cnt]= true;
     }
    }
  }

void rowSet(int rowIn) { //sets chosen row high and all others low
   int cnt;
   for(cnt = 0; cnt < 3; cnt = cnt + 1);
   if(cnt == rowIn){
    digitalWrite(row[cnt], HIGH);
    }
   else{
    digitalWrite(row[cnt], LOW);
    }
   }


//checks every button in the matrix, stores state in buffer 
void scan(){
    int cnt;
    //reads every key, stores in the button buffer array.
    for(cnt = 0; cnt < 3; cnt = cnt + 1){//checks each row
      rowSet(cnt);
      colRead(cnt);
    }
  }

//resets button matrix buffer
void buffReset(){
  int cnt;
  for(cnt = 0; cnt < 9; cnt = cnt + 1){
    button_buff[cnt] = false;
    }   
  }


//DEBUG

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
//uses explicit classes, for testing purposes
buffReset();
rowSet(0);
colRead(0);
bufferRead();
}


