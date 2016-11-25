#include "ArduinoOTA.h"
#include <ESP8266mDNS.h>


//for LED status
//#include <Ticker.h>
//Ticker ticker;


/*void tick()
{
	//toggle state
	int state = digitalRead(LED_PIN);  // get the current state of GPIO1 pin
	digitalWrite(LED_PIN, !state);     // set pin to the opposite state
}*/



// OTA

void otasetup() {
	

    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setHostname("otaTEST123");
    ArduinoOTA.setPassword((const char *) OTA_PASS);
	
    ArduinoOTA.onStart([]() {
        Serial.println("[OTA] Start\n");

		//ticker.attach(0.1, tick);
		
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("\n[OTA] End\n");

		//ticker.detach();
		digitalWrite(LED_PIN, HIGH);
		ESP.reset();
		delay(1000);
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.print("[OTA] Progress: ");
        Serial.print(progress / (total / 100));
        Serial.println("%");
		
		
    });

    ArduinoOTA.onError([](ota_error_t error) {
        #if DEBUG_PORT
            Serial.println("\n[OTA] Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed\n");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed\n");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed\n");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed\n");
            else if (error == OTA_END_ERROR) Serial.println("End Failed\n");
        #endif
    });

    ArduinoOTA.begin();

}

void otaloop() {
    ArduinoOTA.handle();
}
