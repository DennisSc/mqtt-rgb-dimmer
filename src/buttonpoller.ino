#include <FS.h>
#include <Arduino.h>
#include <WifiManager.h>


bool PressStarted;
int counter;


void buttonpollersetup()
{
	pinMode(0, INPUT_PULLUP);
	PressStarted = false;
	counter = 0;
}

void buttonpollerloop()
{
	if (PressStarted = true)
	{
		if (digitalRead(0) == LOW)
		{
			counter++;
			Serial.print("button press count: "); 
			Serial.println(counter);
		}
		else
		{
			counter = 0;
			PressStarted = false;
		}

	}
	else if (digitalRead(0) == LOW) {
		PressStarted = true;
		Serial.println("button start");
	}
		
	else {
		PressStarted = false;
		counter = 0;
	}

	if (counter > 5000)
	{
		SPIFFS.format();
		Serial.println("button caused reset - will restore factory defaults");
		counter = 0;
		PressStarted = false;
		WiFiManager wifiManager;
		wifiManager.resetSettings();
		Serial.println("cleared wifi creds");
		SPIFFS.format();
		Serial.println("formatted SPIFFS - rebooting...");
		delay(3000);
		//reset and try again, or maybe put it to deep sleep
		ESP.reset();
		delay(5000);
	}
}