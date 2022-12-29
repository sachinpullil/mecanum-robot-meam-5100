bool freqPin = false;
int freqLeftPin = 12;  // miso on Atmega
int freqRightPin = 13; // mosi on Atmega
double timerVal = 0.0;
bool bBeaconDetected = false;
bool bRotating = false;

int grab1 = 60;       // adjustable servo positions
int grab2 = 25;
int out1 = 40; 
int out2 = 45;

int pinIR = 14;
//pin 9 for 23Hz or 700Hz indication if needed

hw_timer_t *timer = NULL;   

void move(char dir, int left, int right) {

  //Only proceed if not a repeated call
  if(dir != sentData.cDir)
    sentData.cDir = dir;
  else
    return;
  sentData.nLeftPowerPercent = left;
  sentData.nRightPowerPercent = right;

  SendI2CData();
}

void closeClaws(){
  while(1){
    ledcWrite(1,grab1);
    ledcWrite(2,grab2);
  }
}

void openClaws(){
  ledcWrite(1,out1);
  ledcWrite(2,out2);
}

void IRAM_ATTR ISR() {

  //Debug stuff
  static int ii=0;
  Serial.printf("Inside ISR - %d\n", ii++);
  
  //If we're rotating, stop rotating
  if (true == bRotating){
    bBeaconDetected = true;
    detachInterrupt(freqLeftPin);
  }
}

void beaconTracking(){ 
  hw_timer_t *timer = NULL;                 // set timer to count up at
  timer = timerBegin(0,80000,true);         // 80MHz/80,000=1000Hz so counter value is in ms

  //Move a bit away from the corner then get started
  move('l',10,10);
  delay(1000);
  attachInterrupt(freqLeftPin, ISR, HIGH);
  
  //While in beacon tracking mode
  while(true == bBeaconTracking) {
    while(false == bBeaconDetected && true == bBeaconTracking)
    {
      //Move forward a bit
      move('f',20,20);
      delay(1000);
      
      if(false == bBeaconDetected)
        Serial.println("False");
      else
        Serial.println("True");
      
      bRotating = true;

      //Rotate a half circle left
      for(int ii=1;ii<=10;ii++)
      {
        move('+',1,1);
        delay(100);
        if(true == bBeaconDetected) break;
      }
      if(true == bBeaconDetected) break;

      //Rotate a full circle right
      for(int ii=1;ii<=20;ii++)
      {
        move('-',1,1);
        delay(100); // enough for a full circle approx.
        if(true == bBeaconDetected) break;
      }
      if(true == bBeaconDetected) break;

      //Rotate a half circle left to center once again
      for(int ii=1;ii<=10;ii++)
      {
        move('+',1,1);
        delay(100); // enough for a full circle approx.
        if(true == bBeaconDetected) break;
      }
      if(true == bBeaconDetected) break;

      //Stop
      bRotating = false;
      move('0',0,0);
    }

    //If beacon is detected, move towards it
    if(true == bBeaconDetected)
    {
      move('f',10,10);
    }    
  }
  timerEnd(timer);
}
