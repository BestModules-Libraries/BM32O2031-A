/*****************************************************************
File:         setEID.ino
Description:  Set the EID of the module.
connection method： BMCOM1
******************************************************************/
#include <bm32o2031-a.h>

BM32O2031_A myRGB;              //define object, use HW Serial1 on BMduino
// the setup function runs once when you press reset or power the board
uint8_t EID=1;
void setup() {
  Serial.begin(115200);
  myRGB.begin(); 
//  delay(2000); 
  if(myRGB.setEID(EID) == SUCCESS)
  {
    Serial.println("Setting moduleEID  success");
  }
  else
  {
    Serial.println("Setting moduleEID  fail");
  }
  delay(3000);
    Serial.print("ver:");
  Serial.println(myRGB.getFWVer(EID),HEX);

}

void loop() {
  // put your main code here, to run repeatedly:

}
