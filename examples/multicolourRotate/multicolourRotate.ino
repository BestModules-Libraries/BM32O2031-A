/*****************************************************************
File:         multicolourRotate.ino
Description:  The two modules set the multicolour rotation lamp effect, which is started by the syncaction command at the same time.
connection method： BMCOM1
******************************************************************/
#include <bm32o2031-a.h>

BM32O2031_A myRGB;              //define object, use HW Serial1 on BMduino
uint8_t EID=1; 
// the setup function runs once when you press reset or power the board
void setup() {

	Serial.begin(115200);
  myRGB.begin();  

	if(myRGB.setMulticolourRotate(EID, CLOCKWISE, 255, 10, TRUE, TRUE) == SUCCESS)
	{
		 Serial.println("Setting module 1 success");
	}
	else
	{
		Serial.println("Setting module 1 fail");
	}
	myRGB.syncaction();
	Serial.println("Syncaction");
}

// the loop function runs over and over again forever
void loop() {

}
