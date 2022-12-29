/*HTC Vive*/
#include <math.h>
#include "vive510.h"

#define SIGNALPINLEFT 5 // pin receiving signal from Left Vive circuit
#define SIGNALPINRIGHT 6 // pin receiving signal from Right Vive circuit

Vive510 vive1(SIGNALPINLEFT);
Vive510 vive2(SIGNALPINRIGHT);

uint16_t xLeft,yLeft;
uint16_t xRight,yRight;
int nPrevPrevAngle=1000;
int nPrevAngle=-1000;
int nAngle;

int movingAvg(int newvalue,int WEIGHT) {
static int lastvalue;
lastvalue = (WEIGHT*lastvalue + newvalue)/(WEIGHT+1);
return lastvalue;
}

/*ESPNow*/
#include <esp_now.h>

esp_now_peer_info_t staffcomm = {
  .peer_addr = {0x84,0xF7,0x03,0xA9,0x04,0x78}, 
  .channel = 1,             // channel can be 1 to 14, channel 0 means current channel.
  .encrypt = false,
};

//esp_now_peer_info_t mainS2 = {
//  .peer_addr = {0x84,0xF7,0x03,0xF7,0x80,0xB6}, 
//  .channel = 1,             // channel can be 1 to 14, channel 0 means current channel. ****Issue with same channel?****
//  .encrypt = false,
//};


void pingstaff() {
  uint8_t teamNum = 24;
  esp_now_send(staffcomm.peer_addr, &teamNum, 1);     
}

//Timer to send Vive communications
hw_timer_t *pStaff_comm_timer = NULL;
//hw_timer_t *pS2_comm_timer = NULL;

void IRAM_ATTR onStaffTimer()
{
  char msg[13];
  msg[12]=0; // be sure string is null terminated
  // store into a string with format ##:####,####, which is team#: x, y 
  sprintf(msg,"%2d:%4d,%4d", 24, xLeft, yLeft); //24 is our team number
  esp_now_send(staffcomm.peer_addr, (uint8_t *) msg, 13);
}

//typedef struct ViveLocalizationMessage {
//    int nX;
//    int nY;
//    int nAngle;
//} ViveLocalizationMessage;
//
//ViveLocalizationMessage viveDataToSend;
//
//void IRAM_ATTR onS2Timer()
//{
//  viveDataToSend.nX = (xLeft + yRight)/2;
//  viveDataToSend.nY = (xRight + yLeft)/2;
//  viveDataToSend.nAngle = nAngle;
//  esp_now_send(mainS2.peer_addr, (uint8_t*)&viveDataToSend, sizeof(viveDataToSend));
//  //Serial.printf("Vive Left: (%d, %d)\t Vive Right: (%d, %d) \t Angle: %d\n", xLeft, yLeft, yRight, xRight, nAngle);
//}


void setup() {
  Serial.begin(115200);

  /*HTC Vive*/
  vive1.begin();
  vive2.begin();
  Serial.println("  Vive trackers started");

  /*Set up ESPNow*/
  WiFi.mode(WIFI_STA);
  
  esp_now_init();      
  esp_now_add_peer(&staffcomm);
  //esp_now_add_peer(&mainS2);

//  viveDataToSend.nX = 0;
//  viveDataToSend.nY = 0;
//  viveDataToSend.nAngle = 0;

  /*Set up timer*/
  pStaff_comm_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(pStaff_comm_timer, &onStaffTimer, true);
  timerAlarmWrite(pStaff_comm_timer, 1000000, true);
  timerAlarmEnable(pStaff_comm_timer); //UNCOMMENT THIS to begin transmitting to staff ESP

//  pS2_comm_timer = timerBegin(1, 80, true);
//  timerAttachInterrupt(pS2_comm_timer, &onS2Timer, true);
//  timerAlarmWrite(pS2_comm_timer, 1000000, true);
//  timerAlarmEnable(pS2_comm_timer);
}
                                 
void loop() {   
  if (vive1.status() == VIVE_RECEIVING) {
    xLeft = vive1.xCoord();
    yLeft = vive1.yCoord();    
  }
  else {
    xLeft=0;
    yLeft=0; 
    switch (vive1.sync(5)) {
      break;
    }
  }

  if (vive2.status() == VIVE_RECEIVING) {
    xRight = vive2.xCoord();
    yRight = vive2.yCoord();
  }
  else {
    xRight=0;
    yRight=0; 
    switch (vive2.sync(5)) {
      break;
    }
  }

  nAngle = movingAvg((int)(atan2(yRight - xLeft, xRight - yLeft)*180/M_PI), 9);
  Serial.printf("Vive Left: (%d, %d)\t Vive Right: (%d, %d) \t Angle: %d\n", xLeft, yLeft, yRight, xRight, nAngle);
    
  delay(20);
}
