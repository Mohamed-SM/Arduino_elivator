#include<LinkedList.h>

//the state change of the fc 
boolean lastUpButton = LOW;
boolean courentUpButton = LOW;
boolean lastDownButton = LOW;
boolean courentDownButton = LOW;
//****************

//the state change of the levels Buttons 
boolean last0Button = LOW;
boolean courent0Button = LOW;
boolean last1ButtonUP = LOW;
boolean courent1ButtonUP = LOW;
boolean last1ButtonDOWN = LOW;
boolean courent1ButtonDOWN = LOW;
boolean last2Button = LOW;
boolean courent2Button = LOW;
//****************

//the state change of the fc of the door
boolean lastOpenButton = LOW;
boolean courentOpenButton = LOW;
boolean lastCloseButton = LOW;
boolean courentCloseButton = LOW;
//****************

// the cournet level
char level = 1;

//the stat of the dore
char door = 0;

//the button door buffer
LinkedList<char> buttons_beffer = LinkedList<char>();



//** define the input and the output pins
const int outputPinsNumber = 11;
int outputPins[outputPinsNumber] = {31,33,30,32,41,43,45,47,49,51,53};
const int inputPinsNumber = 9;
int inputPins[inputPinsNumber] = {23,26,27,28,29,34,35,36,37};
//***************

void setup() {

  Serial.begin(9600);

  // the output and the input min mode loops through
  for(int i = 0; i < outputPinsNumber ; i++){
    pinMode(outputPins[i],OUTPUT);
    digitalWrite(outputPins[i],LOW);
  }
  for(int i = 0; i < inputPinsNumber ; i++){
    pinMode(inputPins[i],INPUT); 
  }
  //****************************
  seg7();
}

void loop() {
  seg7();
  
  detect_Button(0);
  detect_level();
  
  if(buttons_beffer.size()){
    goTO(buttons_beffer.get(0));
  }
  
  
}


//the 7 seglents leds function
void seg7(){
  switch(level){
    case 0:
      digitalWrite(41,LOW);
      digitalWrite(43,HIGH);
      digitalWrite(45,HIGH);
      digitalWrite(47,HIGH);
      digitalWrite(49,HIGH);
      digitalWrite(51,HIGH);
      digitalWrite(53,HIGH);
      break;
    case 1:
      digitalWrite(41,LOW);
      digitalWrite(43,LOW);
      digitalWrite(45,LOW);
      digitalWrite(47,LOW);
      digitalWrite(49,HIGH);
      digitalWrite(51,HIGH);
      digitalWrite(53,LOW);
      break;
    case 2:
      digitalWrite(41,HIGH);
      digitalWrite(43,LOW);
      digitalWrite(45,HIGH);
      digitalWrite(47,HIGH);
      digitalWrite(49,LOW);
      digitalWrite(51,HIGH);
      digitalWrite(53,HIGH);
      break;

   default:
      digitalWrite(41,LOW);
      digitalWrite(43,LOW);
      digitalWrite(45,LOW);
      digitalWrite(47,LOW);
      digitalWrite(49,LOW);
      digitalWrite(51,LOW);
      digitalWrite(53,LOW);  
  }
}

// the debounce function to detect the change in the fc or buttons
boolean debounce(boolean last, int pin){
  boolean courent = digitalRead(pin);
  if(last != courent){
    delay(5);
    courent = digitalRead(pin);
  }
  return courent;
}


//this function to detect the level change unsing the fc
void detect_level(){
  courentDownButton = debounce(lastDownButton,36);
  if(lastDownButton == LOW && courentDownButton == HIGH){
    Serial.println("DOWN");
    level--;
  }
  lastDownButton = courentDownButton;

  courentUpButton = debounce(lastUpButton,34);
  if(lastUpButton == LOW && courentUpButton == HIGH){
    Serial.println("UP");
    level++;
  }
  lastUpButton = courentUpButton;
  seg7();
}

//the function to detect the door 
void detect_door(){
  courentOpenButton = debounce(lastOpenButton,37);
  if(lastOpenButton == LOW && courentOpenButton == HIGH){
    Serial.println("OPEN");
    door = 1;
  }
  lastOpenButton = courentOpenButton;

  courentCloseButton = debounce(lastCloseButton,35);
  if(lastCloseButton == LOW && courentCloseButton == HIGH){
    Serial.println("CLOSE");
    door = 0;
  }
  lastCloseButton = courentCloseButton;
  
}

// go to the specific level
void goTO(char goToLevel){
  
  char directionPin = 0;

  //decied the direction  pin
  if(level > goToLevel){
     directionPin = 32;
  }else{
    if(level < goToLevel){
      directionPin = 30;  
    }
    
  }

  if(directionPin){
    digitalWrite(directionPin,HIGH);
  //keep detecting level until it is equeal to the level we want to go to
  while(level != goToLevel){
    detect_Button(0);
    detect_level();
  }

  //stop the derection pin
  digitalWrite(directionPin,LOW);
  }

  doorOpenAndClose();
}

// go to the specific level
void doorOpenAndClose(){
  if(!door){
    digitalWrite(31,HIGH);
    while(!door){
      detect_door();
      detect_Button(0);
      delay(10);
    }
    digitalWrite(31,LOW);
  }

  delay(3000);
  buttons_beffer.shift();

  if(door){
    digitalWrite(33,HIGH);
    while(door){
      detect_door();
      detect_Button(1);
      delay(10);
    }
    digitalWrite(33,LOW);
  }

  for(int i = 0 ; i < 150 ;i++){
    detect_Button(0);
    delay(10);
  }
  
}

void detect_Button(boolean fast){
  
  courent0Button = debounce(last0Button,27);
  if(last0Button == LOW && courent0Button == HIGH){
    Serial.println("Zero");
    if(level == 0 && fast){
      fastOpenAndClose();
    }
    else{
      addToBuffer(0);  
    }
    
       
  }
  last0Button = courent0Button;

  courent1ButtonUP = debounce(last1ButtonUP,28);
  if(last1ButtonUP == LOW && courent1ButtonUP == HIGH){
    Serial.println("One UP");
    if(level == 1 && fast){
      fastOpenAndClose();
    }
    else{
      addToBuffer(1);  
    }
  }
  last1ButtonUP = courent1ButtonUP;
  
  courent1ButtonDOWN = debounce(last1ButtonDOWN,29);
  if(last1ButtonDOWN == LOW && courent1ButtonDOWN == HIGH){
    Serial.println("One DOWN");
    if(level == 1 && fast){
      fastOpenAndClose();
    }
    else{
      addToBuffer(1);  
    }
  }
  last1ButtonDOWN = courent1ButtonDOWN;
  
  courent2Button = debounce(last2Button,26);
  if(last2Button == LOW && courent2Button == HIGH){
    Serial.println("TOW");
    if(level == 2 && fast){
      fastOpenAndClose();
    }
    else{
      addToBuffer(2);  
    }
  }
  last2Button = courent2Button;
  
}

void addToBuffer(int n){
  char flag = 0;
    for(int i = 0 ; i < buttons_beffer.size(); i++){
      if(buttons_beffer.get(i) == n ){
        flag = 1;
        break;
      }
    }
    
    if(!flag) buttons_beffer.add(n);
}


void fastOpenAndClose(){
  door = 0;
  digitalWrite(33,LOW);
  delay(50);
  digitalWrite(31,HIGH);
    while(!door){
      detect_door();
      detect_Button(0);
      delay(10);
    }
  digitalWrite(31,LOW);
  
  delay(3000);

  if(door){
    digitalWrite(33,HIGH);
  }
}












