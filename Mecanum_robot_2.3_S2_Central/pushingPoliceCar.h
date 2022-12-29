void pushPoliceCar()
{
  static int doOnce = 1;
  if(doOnce)
  {
    //Angle towards car
    sentData.cDir = '+';
    sentData.nLeftPowerPercent = 10;
    sentData.nRightPowerPercent = 10;
    SendI2CData();
    delay(100);
    sentData.cDir = '0';
    SendI2CData();
    doOnce = 0;
  }

  //Push forward
  sentData.cDir = 'f';
  sentData.nLeftPowerPercent = 100;
  sentData.nRightPowerPercent = 100;

  SendI2CData();
}
