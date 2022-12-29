/* Team 24 - Anthony Saldutti, Christopher Chong, Sachin Pullil
 * 
 * The .ino file that drives the wheel motors.
 * 
 */

#include <WiFi.h>

/*I2C Setup*/
#include "myi2csetup.h"

/*Motor control*/
//Pin definitions
#define FWDTL 7
#define FWDTR 6
#define FWDBL 5
#define FWDBR 4
#define PWMPINLEFT 18
#define PWMPINRIGHT 1

//LEDC definitions
#define LEDC_CHANNEL_LEFT 0
#define LEDC_CHANNEL_RIGHT 1
#define BASE_FREQUENCY 30
#define STD_RESOLUTION 12

//PWM variables
int nPWMLeft = 0;
int nPWMRight = 0;
int maxLEDC = ((1<<STD_RESOLUTION)-1);

void setup() {
  Serial.begin(115200);

  /*I2C setup*/
  i2c_slave_init();
  
  /*Motor control*/
  pinMode(FWDTL,OUTPUT);
  pinMode(FWDTR,OUTPUT);
  pinMode(FWDBL,OUTPUT);
  pinMode(FWDBR,OUTPUT);

  digitalWrite(FWDTL,HIGH);
  digitalWrite(FWDTR,HIGH);
  digitalWrite(FWDBL,HIGH);
  digitalWrite(FWDBR,HIGH);

  ledcAttachPin(PWMPINLEFT, LEDC_CHANNEL_LEFT);
  ledcSetup(LEDC_CHANNEL_LEFT, BASE_FREQUENCY, STD_RESOLUTION);

  ledcAttachPin(PWMPINRIGHT, LEDC_CHANNEL_RIGHT);
  ledcSetup(LEDC_CHANNEL_RIGHT, BASE_FREQUENCY, STD_RESOLUTION);

  ledcWrite(LEDC_CHANNEL_LEFT, 0);
  ledcWrite(LEDC_CHANNEL_RIGHT, 0);
}

void loop() {

  /*I2C read*/
  //I2Cport buffer length of data  max ticks to wait
  if (i2c_slave_read_buffer(I2C_NUM_0, (uint8_t*)&receivedData, sizeof(receivedData), 0) > 0 ) { // last term is timeout period, 0 means don't wait  
    _cDir = receivedData.cDir;
    _nLeftPowerPercent = receivedData.nLeftPowerPercent;
    _nRightPowerPercent = receivedData.nRightPowerPercent;
    Serial.printf("Received from master: %c direction, %d left power, %d right power\n",receivedData.cDir, receivedData.nLeftPowerPercent, receivedData.nRightPowerPercent); 
  }

    
  /*Actuate motors*/
  nPWMLeft = map(_nLeftPowerPercent, 0, 100, 0.24*maxLEDC, maxLEDC);
  nPWMRight = map(_nRightPowerPercent, 0, 100, 0.24*maxLEDC, 0.9*maxLEDC);
  
  if('0' == _cDir)
  {
    ledcWrite(LEDC_CHANNEL_LEFT, 0);
    ledcWrite(LEDC_CHANNEL_RIGHT, 0);
  }
  else
  {
    ledcWrite(LEDC_CHANNEL_LEFT, nPWMLeft);
    ledcWrite(LEDC_CHANNEL_RIGHT, nPWMRight);
    
    switch(_cDir)
    {
      case 'f':
        digitalWrite(FWDTL,HIGH);
        digitalWrite(FWDTR,HIGH);
        digitalWrite(FWDBL,HIGH);
        digitalWrite(FWDBR,HIGH);
        break;
      case 'b':
        digitalWrite(FWDTL,LOW);
        digitalWrite(FWDTR,LOW);
        digitalWrite(FWDBL,LOW);
        digitalWrite(FWDBR,LOW);
        break;
      case 'l':
        digitalWrite(FWDTL,LOW);
        digitalWrite(FWDTR,HIGH);
        digitalWrite(FWDBL,HIGH);
        digitalWrite(FWDBR,LOW);
        break;
      case 'r':
        digitalWrite(FWDTL,HIGH);
        digitalWrite(FWDTR,LOW);
        digitalWrite(FWDBL,LOW);
        digitalWrite(FWDBR,HIGH);
        break;
      case '+':
        digitalWrite(FWDTL,LOW);
        digitalWrite(FWDTR,HIGH);
        digitalWrite(FWDBL,LOW);
        digitalWrite(FWDBR,HIGH);
        break;
      case '-':
        digitalWrite(FWDTL,HIGH);
        digitalWrite(FWDTR,LOW);
        digitalWrite(FWDBL,HIGH);
        digitalWrite(FWDBR,LOW);
        break;
      default:
        break;
    }  
  }
}
