#include <FS.h>  
#include <ArduinoJson.h> 

#include <Arduino.h>
#include <ESP8266WiFi.h>

//needed for wifimanager library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager



//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback() {
	Serial.println("Saving config...");
	shouldSaveConfig = true;
}




void wifisetup()
{
    Serial.println("Booting");
	
	
	





	//clean FS, for testing
	//SPIFFS.format();










	//read configuration from FS json
	Serial.println("mounting FS...");
	
	if (SPIFFS.begin()) {
		Serial.println("mounted FS ok");
	

		if (SPIFFS.exists("/config.json")) {

			//file exists, reading and loading
			Serial.println("reading config file");
	
			File configFile = SPIFFS.open("/config.json", "r");
			if (configFile) {

				Serial.println("opened config file");
				size_t size = configFile.size();

				// Allocate a buffer to store contents of the file.
				std::unique_ptr<char[]> buf(new char[size]);

				configFile.readBytes(buf.get(), size);
				DynamicJsonBuffer jsonBuffer;
				JsonObject& json = jsonBuffer.parseObject(buf.get());
				json.printTo(Serial);
				if (json.success()) {
					Serial.println("\nparsed json");

					
					strcpy(mqtt_server, json["mqtt_server"]);
					//Serial.println("read mqtt_server from json: " + mqtt_server);
					strcpy(mqtt_port, json["mqtt_port"]);
					//Serial.println("read mqtt_port from json: " + mqtt_port);
					strcpy(mqtt_topic, json["mqtt_topic"]);
					//Serial.println("read mqtt_topic from json: " + mqtt_topic);
				}
				else {
					Serial.println("failed to load json config");

				}
			}
		}
	}
	else {
		Serial.println("failed to mount FS");

	}
	//end read



	// The extra parameters to be configured (can be either global or just in the setup)
	// After connecting, parameter.getValue() will get you the configured value
	// id/name placeholder/prompt default length
	WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
	WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
	WiFiManagerParameter custom_mqtt_topic("topic", "mqtt topic", mqtt_topic, 60);



	//WiFiManager
	//Local intialization. Once its business is done, there is no need to keep it around
	WiFiManager wifiManager;

	//set config save notify callback
	wifiManager.setSaveConfigCallback(saveConfigCallback);

	//set static ip
	// wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

	//add all your parameters here
	wifiManager.addParameter(&custom_mqtt_server);
	wifiManager.addParameter(&custom_mqtt_port);
	wifiManager.addParameter(&custom_mqtt_topic);











	//reset settings - for testing
	//wifiManager.resetSettings();













	//set minimu quality of signal so it ignores AP's under that quality
	//defaults to 8%
	wifiManager.setMinimumSignalQuality();

	//sets timeout until configuration portal gets turned off
	//useful to make it all retry or go to sleep
	//in seconds
	//wifiManager.setTimeout(120);

	//fetches ssid and pass and tries to connect
	//if it does not connect it starts an access point with the specified name
	//here  "AutoConnectAP"
	//and goes into a blocking loop awaiting configuration
	
	if (!wifiManager.autoConnect()) {
		Serial.println("failed to connect and hit timeout");
		wifiManager.resetSettings();
		SPIFFS.format();
		delay(3000);
		//reset and try again, or maybe put it to deep sleep
		ESP.reset();
		delay(5000);
	}

	//if you get here you have connected to the WiFi
	Serial.println("connected...yeey :)");

	
	//read updated parameters
	strcpy(mqtt_server, custom_mqtt_server.getValue());
	strcpy(mqtt_port, custom_mqtt_port.getValue());
	strcpy(mqtt_topic, custom_mqtt_topic.getValue());

	//save the custom parameters to FS
	if (shouldSaveConfig) {
		Serial.println("saving config");
		DynamicJsonBuffer jsonBuffer;
		JsonObject& json = jsonBuffer.createObject();
		
		json["mqtt_server"] = mqtt_server;
		json["mqtt_port"] = mqtt_port;
		json["mqtt_topic"] = mqtt_topic;

		File configFile = SPIFFS.open("/config.json", "w");
		if (!configFile) {
			Serial.println("failed to open config file for writing");
		}

		json.printTo(Serial);
		json.printTo(configFile);
		configFile.close();
		//end save
	}

	Serial.print("local IP: ");
	Serial.println(WiFi.localIP());



}

void wifiloop(){}
