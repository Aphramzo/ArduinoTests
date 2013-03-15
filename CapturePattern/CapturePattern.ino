const int setButton = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
const int patternButton = 4;
const int numberOfPushesInPattern = 7;
const int recordingLEDPin = 13;

// variables will change:
int setButtonState = 0;         // variable for reading the pushbutton status
int lastSetButtonState = 0;
int patternButtonState = 0;
int lastPatternButtonState = 0;

int isCapturing = 0; //know to listen to the second button for setting the pattern

long patternButtonHoldLength = 0; //number of loops the second button is held
int recordingButtonHold = 0;  //flag to see if we should be listening for the down press of the second button

int pushedThisPattern = 0;

long pattern[numberOfPushesInPattern];

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(setButton, INPUT); 
  digitalWrite(ledPin, LOW);  
  digitalWrite(recordingLEDPin,LOW);
  Serial.begin(9600);
}

void loop(){
  // read the state of the pushbutton value:
  setButtonState = digitalRead(setButton);
  patternButtonState = digitalRead(patternButton);
  
  if(setButtonIsPushed()){
    StartCapturing();
  }  

  
  if(isCapturing){
     if(patternButtonState != lastPatternButtonState && patternButtonState){
       recordingButtonHold = 1;
       pushedThisPattern++;
     }
       
     else if(ReleasedPatternButton()) //this means they just released the second 'pattern' button
        ReleasePatternButton();  
     
     if(recordingButtonHold)
       patternButtonHoldLength++;
     
  }
  
  lastSetButtonState = setButtonState;
  lastPatternButtonState = patternButtonState;
}

//Will only fire ONCE on down press of button. Not on button up either
boolean setButtonIsPushed(){
   return setButtonState != lastSetButtonState && setButtonState; 
}

//because of false holds (because of drunkeness?) we need to make sure we
//dont record pushes of less than 1000 loops
boolean ReleasedPatternButton(){
  return patternButtonState != lastPatternButtonState && patternButtonHoldLength > 1000; 
}

void PatternCompleted(){
  isCapturing = 0;
  pushedThisPattern = 0 ; 
  digitalWrite(recordingLEDPin,LOW);
  printPattern();
}

void ReleasePatternButton(){
  pattern[pushedThisPattern-1] = patternButtonHoldLength;
  patternButtonHoldLength = 0;
  recordingButtonHold = 0;
  
  if(pushedThisPattern==numberOfPushesInPattern)
    PatternCompleted();
         
}

void printPattern(){
   for(int i = 0; i<numberOfPushesInPattern;i++){
      Serial.println(pattern[i]); 
      Serial.println(i);
   }
}

void StartCapturing(){
  isCapturing = 1;
  digitalWrite(recordingLEDPin,HIGH);
}
      
