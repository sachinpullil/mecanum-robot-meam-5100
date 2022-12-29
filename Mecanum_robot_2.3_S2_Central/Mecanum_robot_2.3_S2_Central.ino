/* Team 24 - Anthony Saldutti, Christopher Chong, Sachin Pullil

   Primary .ino file that serves as the center for all robot behavior.

*/

/*HTML interface*/
#include <WiFi.h>
#include "html510.h"
#include "my_html_code.h"

//Sachin's phone hotspot (CHANGE HERE and on C3 code to control from your hotspot)
const char* ssid = "Sachin";
const char* password = "sachin123";
//const char* ssid     = "TP-Link_05AF";
//const char* password = "47543454";
//const char* ssid     = "TP-Link_FD24";
// const char* password = "65512111";

//Initialize HTML server
HTML510Server h(80);

/*ESPNow setup*/
#include "espnowsetup.h" //When you uncomment this, there will be redefinition errors

/*I2C setup*/
#include "mymasteri2csetup.h"

/*Wallfollowing*/
#include "wallFollowing.h"
bool bWallFollowing = false;

/*Beacon tracking*/
bool bBeaconTracking = false;
#include "beacon_tracking.h"

/*Pushing police car*/
bool bPushingPoliceCar = false;
#include "pushingPoliceCar.h"

/*HTML callback functions */
#include "html_cb_fxns.h"


void setup() {
  Serial.begin(115200);

  /*Set up WiFi*/
  WiFi.mode(WIFI_STA);
  Serial.print("Access point: "); Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED )
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  /*Begin HTML server and assign callback functions*/
  h.begin();
  h.attachHandler("/ ", handleRoot);
  h.attachHandler("/hitFollowWall", handleFollowWall);
  h.attachHandler("/hitGrabBeacon", handleGrabBeacon);
  h.attachHandler("/hitPushPoliceCar", handlePushPoliceCar);
  h.attachHandler("/output", handleOutput);
  h.attachHandler("/vivelocation", handleViveLocationOutput);

  h.attachHandler("/hitForward", hitForward);
  h.attachHandler("/hitBackward", hitBackward);
  h.attachHandler("/hitLeft", hitLeft);
  h.attachHandler("/hitRight", hitRight);
  h.attachHandler("/hitClockwise", hitClockwise);
  h.attachHandler("/hitCounterClockwise", hitCounterClockwise);
  h.attachHandler("/hitStop", hitStop);

  /*Initialize I2C*/
  i2c_master_init();

//  /*Initialize ESPNow*/
//  Serial.print("ESPNow Receiving MAC: "); Serial.println(WiFi.macAddress());
//  if(esp_now_init() != ESP_OK)
//  {
//    Serial.println("init failed");
//  }
//  esp_now_register_recv_cb(OnDataRecv);

  /*Initialize Sensors*/
  pinMode(LEFT_trigPin, OUTPUT);
  pinMode(LEFT_echoPin, INPUT);   // initialize left ultrasonic

  pinMode(BACK_trigPin, OUTPUT);
  pinMode(BACK_echoPin, INPUT);   // initialize back ultrasonic

  pinMode(RIGHT_trigPin, OUTPUT);
  pinMode(RIGHT_echoPin, INPUT);  // initialize right ultrasonic

  //pinMode(IR_Pin, INPUT);       // initialize IR

  /*Beacon tracking*/
  pinMode(12,INPUT);
  pinMode(13,INPUT);
  ledcSetup(1,50,10); //code to initialize servos, change pins if needed
  ledcAttachPin(3,1); //ADD TO MAIN FILE
  ledcSetup(2,50,10);
  ledcAttachPin(8,2);
}


void loop() {
  if (true == bWallFollowing)
  {
    sensorReadings();
    wallFollowing();
  }
  else if(true == bBeaconTracking)
  {
    beaconTracking();
  }
  else if(true == bPushingPoliceCar)
  {
    pushPoliceCar();
  }

  //Run HTML server
  h.serve();
  delay(10);
}
