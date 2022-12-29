

//---------------------------- SENSING PINS ----------------------------

#define BACK_echoPin 10
#define BACK_trigPin 11       // back ultrasonic

#define LEFT_echoPin 15
#define LEFT_trigPin 16       // left ultrasonic

#define RIGHT_echoPin 17
#define RIGHT_trigPin 18      // right ultrasonic

//----------------------------- VARIABLES -----------------------------

long duration;                    // variable for the duration of sound wave travel
int ultrasonicDist;               // variable for ultrasonic distance measurement

int nFrontUltrasonicDist = 1000;               // variable for front ultrasonic distance measurement

int nPrevFrontUltrasonicDist = 0;               // variable for prev front ultrasonic distance measurement

int frontDist=100;

int forwardFlag = 0;
int bObstacleDetectionCount = 0;

//----------------------- MOVING AVERAGE FILTER -----------------------
int movingAvg(int newvalue,int lastvalue) {
int weightedAvg = (9*lastvalue + newvalue)/(9+1); //I usually use 9 as weight
return weightedAvg;
}

//----------------------- ULTRASONIC SUBROUTINE -----------------------

void ultrasonic(int echoPin, int trigPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;    // speed of sound wave divided by 2 (go and back)

  if(distance > 1 && distance < 40)       // only record distances below 20cm
  {
    if(distance - nPrevFrontUltrasonicDist < 20) //reduce spikes of data readings
    {
      nFrontUltrasonicDist = movingAvg(distance,nPrevFrontUltrasonicDist);
      nPrevFrontUltrasonicDist= distance;
    }
  }

  else
  {
    //Average it with a large value like 100
    nFrontUltrasonicDist = movingAvg(100,nPrevFrontUltrasonicDist);
    nPrevFrontUltrasonicDist= 100;
  }
}

//------------------------- MOVE SUBROUTINE -------------------------

void moveFlipped(char dir, int percentage) 
{
  switch(dir)
  {
    case 'f':
      dir = 'b';
      break;
    case 'b':
      dir = 'f';
      break;
    case 'l':
      dir = 'r';
      break;
    case 'r':
      dir = 'l';
      break;
    default:
      break;
  }  
  
  sentData.cDir = dir;
  sentData.nLeftPowerPercent = 1;
  sentData.nRightPowerPercent = 1;

  SendI2CData();
}

//------------------------- RESET ------------------------
void ResetFlags()
{
  nFrontUltrasonicDist = 1000;               // variable for front ultrasonic distance measurement
  nPrevFrontUltrasonicDist = 0;               // variable for prev front ultrasonic distance measurement
  frontDist=100;
  
  forwardFlag = 0;
  bObstacleDetectionCount = 0;
}

//------------------------- SENSOR SUBROUTINE ------------------------

void sensorReadings()
{
  /*Note that everything is flipped because we wall-follow backwards*/
  
  ultrasonic(BACK_echoPin, BACK_trigPin);       //Back Ultrasonic
  frontDist = nFrontUltrasonicDist;

}

//--------------------- WALLFOLLOWING SUBROUTINE --------------------

void wallFollowing()
{
  Serial.print("Front: ");
  Serial.print(frontDist);
  Serial.print("\t");
  
  if(frontDist > 20)        //sensing forward
  {
    if(0 == forwardFlag)    
    {
      moveFlipped('f',20);
      Serial.println("forward");

      forwardFlag = 1;
    }
    bObstacleDetectionCount = 0;
  }
  else
  {
    bObstacleDetectionCount++;
    if(bObstacleDetectionCount > 6)
    {
      //Scoot back
      moveFlipped('b',20);
      delay(500);
      //Scoot left
      moveFlipped('l',20);
      delay(500);
      //Rotate left
      moveFlipped('+',40);
      delay(550);
      forwardFlag = 0;
      bObstacleDetectionCount = 0;
    }
  }
}
