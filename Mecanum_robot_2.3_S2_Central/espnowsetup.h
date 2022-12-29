//#include <esp_now.h>
//
//typedef struct ViveLocalizationMessage {
//    int nX;
//    int nY;
//    int nAngle;
//} ViveLocalizationMessage;
//
//ViveLocalizationMessage viveDataReceived;
//
////callback on receive
//void OnDataRecv(const uint8_t *mac_addr, const uint8_t* data, int data_len)
//{
//  memcpy(&viveDataReceived, data, data_len);
//  Serial.printf("Vive data received. X = %d, Y = %d, Angle = %d\n", viveDataReceived.nX, viveDataReceived.nY, viveDataReceived.nAngle);
//}


//esp_now_peer_info_t peerMotorC3 = {
//  .peer_addr = {0x60, 0x55, 0xF9, 0x57, 0x44, 0xE4}, //receiver MAC address
//  .channel = 0,
//  .encrypt = false,
//};

//typedef struct MotorControlMessage {
//    char cDir;
//    int nLeftPowerPercent;
//    int nRightPowerPercent;
//} MotorControlMessage;
//
//MotorControlMessage sentData;
//
//bool bSendSuccess = false;

//void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
//{
//  if(status == ESP_NOW_SEND_SUCCESS)
//  {
//    Serial.println("Success.");
//    bSendSuccess = true;
//  }
//  else
//  {
//    //Serial.println(esp_err_to_name(status));
//    bSendSuccess = false;
//  }
//}

//void SendUntilSuccess()
//{
//  int ii=0;
//  while(false == bSendSuccess)
//  {
//    esp_err_t myErr = esp_now_send(peerMotorC3.peer_addr, (uint8_t*)&sentData, sizeof(sentData));
////    if(myErr == ESP_OK)
//      //Serial.println("Sent");
//    if(++ii == 50 && false == bSendSuccess)
//    {
//      //Serial.println("Failed to send due to timeout.");
//      break;
//    }      
//  }
//  bSendSuccess = false;
//}
