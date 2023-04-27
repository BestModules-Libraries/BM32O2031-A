/*****************************************************************
File:         bicolorBlink.ino
Description:  Two color blink light effect, switching color combination every 10s
connection method： BMCOM1
******************************************************************/

#include <bm32o2031-a.h>

BM32O2031_A RGB_M;              //define object, use HW Serial1 on BMduino
//BM32O2031_A     RGB_M(&Serial1); //Please uncomment out this line of code if you use HW Serial1 on BMduino
//BM32O2031_A     RGB_M(&Serial2); //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BM32O2031_A     RGB_M(&Serial3); //Please uncomment out this line of code if you use HW Serial3 on BMduino
//BM32O2031_A     RGB_M(&Serial4); //Please uncomment out this line of code if you use HW Serial4 on BMduino

uint32_t color_data[][2] =
{
	{RED, ORANGE}, {YELLOW, GREEN}, {CVAN, PURPLE}, {BLUE, WHITE},
};

// the setup function runs once when you press reset or power the board
void setup() {

	Serial.begin(115200);
  RGB_M.begin();   
}

// the loop function runs over and over again forever
void loop() {

	uint8_t i;
	for(i = 0; i < 4; i++)
	{
		if(RGB_M.setBicolorBlink(1, color_data[i][0], color_data[i][1]) == SUCCESS)
		{
			Serial.println("Setting success");
		}
		else
		{
			Serial.println("Setting fail");
		}
		delay(10000);
	} 

}
