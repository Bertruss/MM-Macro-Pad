//9 button matrix with 7-seg output
//prints which button has been pressed to 7-seg display

// pin assignment
const int col[3] = {1, 2, 3};
const int row[3] = {9, 10, 11};

//7-seg pin map
const int pin[9] = {14,15,16,17,18,19,20,21,22};

//codes for 7-seg, based upon above pin layout
//refer to Single_Digit_7_seg.ino for more detailed description
const char numCodes[][10] = {
  "110110101", //0
  "000000101", //1
  "100110011", //2
  "100010111", //3
  "010000111", //4
  "110010110", //5
  "110110110", //6
  "100000101", //7
  "110110111", //8
  "110000111"  //9
  };

//consider this a beta implementation. 
//Working on just having a list of integer representations of the keys
bool button_buff[9] = {false,false,false,false,false,false,false,false,false};

////////////////////////////////Button Matrix Code/////////////////////////////////////////
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
    digitalWrite(row[rowIn], HIGH);
    for(cnt = 1; cnt < 3; cnt = cnt + 1)
    { 
      rowIn++;
      if(rowIn > 3){rowIn = 1;}
      digitalWrite(row[rowIn], LOW);
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
/////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////7 Seg Display Code//////////////////////////////////////////////////////
void dispNum(int Num){//uses data from numCodes to display input number, 0 through 9
    if(Num >= 0 && Num <= 9){
      int cnt;
      for (cnt = 0; cnt < 9; cnt = cnt + 1){ //scans through numcode, sets appropriate pins high/low
        if(numCodes[Num][cnt] == '1'){
          digitalWrite(pin[cnt],LOW);
        }
        else{
          digitalWrite(pin[cnt],HIGH);    
        }
      } 
    }
 }

void dispReset(){
    int cnt;
    for (cnt = 0; cnt < 9; cnt = cnt + 1){ //resets all segments  
       digitalWrite(pin[cnt],HIGH);
    } 
  }

void dispNumDur(int Num, int runtime){//reduces constant load on chip. runtime is in 20's of ms. runtime 50 = 1s;
    int cnt;
    for(cnt = 0; cnt < runtime; cnt = cnt + 1){
      dispNum(Num);
      delay(10);
      dispReset();
      delay(10);
      }
  }

//////////////////////////////////////////////////////////////////////////////////////////////


//DEBUG
void bufferRead(){ //only reads one key at a time, but this is just debug
    int cnt;
    for(cnt = 0; cnt < 8; cnt = cnt + 1){
        if(button_buff[cnt]){
            Keyboard.print((cnt + 1));
          }    
      }
   Keyboard.println();
   }

void setup() {
  int cnt;
    //button matrix instantiation
  for (cnt = 0; cnt < 3; cnt = cnt + 1){ //instantiates every pin in row[] to OUTPUT and in col[] to INPUT_PULLUP
      pinMode(row[cnt],OUTPUT);
      pinMode(col[cnt],INPUT);
    }

    //7 seg instantiation
  for (cnt = 0; cnt < 9; cnt = cnt + 1){ //instantiates every pin in pin[] to OUTPUT
     pinMode(pin[cnt],OUTPUT);
     digitalWrite(pin[cnt],HIGH);
  }
  
}

void loop() {
//uses explicit classes, for testing purposes
buffReset();
rowSet(0);
colRead(0);
bufferRead();  
delay(5000);
}


