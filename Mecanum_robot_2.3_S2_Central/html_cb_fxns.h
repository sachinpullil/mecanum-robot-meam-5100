void handleRoot()
{
  h.sendhtml(body); //Defined in my_html_code.h
}

void hitForward() {
  sentData.cDir = 'f';
  sentData.nLeftPowerPercent = 100;
  sentData.nRightPowerPercent = 100;

  SendI2CData();
  
  h.sendplain(body);
}

void hitBackward() {
  sentData.cDir = 'b';
  sentData.nLeftPowerPercent = 100;
  sentData.nRightPowerPercent = 100;
  
  SendI2CData();
    
  h.sendplain(body);
}

void hitLeft() {
  sentData.cDir = 'l';
  sentData.nLeftPowerPercent = 100;
  sentData.nRightPowerPercent = 100;

  SendI2CData();
    
  h.sendplain(body);
}

void hitRight() {
  sentData.cDir = 'r';
  sentData.nLeftPowerPercent = 100;
  sentData.nRightPowerPercent = 100;

  SendI2CData();
    
  h.sendplain(body);
}

void hitClockwise() {
  sentData.cDir = '-';
  sentData.nLeftPowerPercent = 10;
  sentData.nRightPowerPercent = 10;

  SendI2CData();
    
  h.sendplain(body);
}

void hitCounterClockwise() {
  sentData.cDir = '+';
  sentData.nLeftPowerPercent = 10;
  sentData.nRightPowerPercent = 10;

//  SendI2CData();
//  delay(50);
//  sentData.cDir = '0';
//  SendI2CData();
  
  h.sendplain(body);
}

void hitStop() {
  sentData.cDir = '0';
  sentData.nLeftPowerPercent = 0;
  sentData.nRightPowerPercent = 0;

  SendI2CData();
    
  h.sendplain(body);
}

void handleOutput()
{
  String s = "Nothing is happening.";
  if(true == bWallFollowing)
    s = "Wallfollowing activated";
  h.sendplain(s);
}

void handleViveLocationOutput()
{
  //int nX = viveDataReceived.nX, nY = viveDataReceived.nY, nAngle = viveDataReceived.nAngle;
  //String s = "X = " + String(nX) + ", Y = " + String(nY) + ", Angle = " + String(nAngle);
  String s = "";
  h.sendplain(s);
}

void handleFollowWall()
{
  if(false == bWallFollowing)
    bWallFollowing = true;
  else
    bWallFollowing = false;

  ResetFlags();
  
  h.sendplain(body);
}

void handleGrabBeacon()
{
  if(false == bBeaconTracking)
    bBeaconTracking = true;
  else
    bBeaconTracking = false;
  
  h.sendplain(body);
}

void handlePushPoliceCar()
{
  if(false == bPushingPoliceCar)
    bPushingPoliceCar = true;
  else
    bPushingPoliceCar = false;
  
  h.sendplain(body);
}
