//9 button matrix

// pin assignment
const int col[3] = {1, 2, 3};
const int row[3] = {9, 10, 11};

bool button_buff[3][3] = {
  {false,false,false},
  {false,false,false},
  {false,false,false}
  };

void colRead(int row){ //handles column polling
  int cnt;
  for(cnt = 0; cnt < 3; cnt = cnt + 1){
     button_buff[row][cnt] = digitalRead(cnt) == HIGH;
    }
  }

void rowSet(int rowC) { //sets chosen row high and all others low
/*
  //method 1, harder to read, very short, computational overhead
   
   if (row <= 3 && row >= row1){
    int rowS = row[rowC];
    int cnt;
    digitalWrite(row, HIGH);
    for(cnt = 1; cnt < 3; cnt = cnt + 1)
    { 
      row++;
      if(rowSelect > row[2]){rowSelect = row[0];}
      digitalWrite(rowSelect, LOW);
    }
   }
 */
 
  //method 2, simpler to read, very long, memory overhead
  switch(rowC){
    case 0 : 
      digitalWrite(row[0], HIGH);
      digitalWrite(row[1], LOW);
      digitalWrite(row[2], LOW);
    break;
    
    case 1 : 
      digitalWrite(row[0], LOW);
      digitalWrite(row[1], HIGH);
      digitalWrite(row[2], LOW);
    break;

    case 2 :
      digitalWrite(row[0], LOW);
      digitalWrite(row[1], LOW);
      digitalWrite(row[2], HIGH);
    break;
    
    default :
      digitalWrite(row[0], LOW);
      digitalWrite(row[1], LOW);
      digitalWrite(row[2], LOW);
    }
  }

void printPresses(){
  
  }

void setup() {
  pinMode(row[0], OUTPUT);
  pinMode(row[1], OUTPUT);
  pinMode(row[2], OUTPUT);
  pinMode(col[0], INPUT);
  pinMode(col[1], INPUT);
  pinMode(col[2], INPUT);
}




void loop() {
  int cnt;
  for(cnt = 0; cnt < 3; cnt = cnt + 1){
    rowSet(cnt);
    colRead(cnt);
  }
  //reads every key, stored in the button buffer array.
  //does nothing with this information, atm. 
}


