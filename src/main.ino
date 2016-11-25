//
// mqtt RGB dimmer module, written 2016 by Dennis Schulze    //
// 
// Needs platformio in order to compile, at present time you //
//will also need to download and add a  dependency on  the   //
// library "wifimanager" manually, via platformio cmd line   //
// 
// target platform: esp82xx									 //
// other needed hardware: any length of WS2812x controlled   //
// RGB LED strip, U shaped aluminium rail, one 5V pwr supply //
// depending on the esp82xx module: additional 3,3V supply	 //
//
// other needed software:									 //
// MQTT broker (mosquitto for example)						 //
//


#include <Arduino.h>




//define your default values here, if there are different values in config.json, they are overwritten.
char mqtt_server[40];
char mqtt_port[6] = "1883";
char mqtt_topic[60];


long now;

#include "defaults.h"


void setup() {
	FadeStepCounter = 0;

    hardwaresetup();
    wifisetup();
    otasetup();
	mqttsetup();
	dimmersetup();

}

void loop() {
    hardwareloop();
    wifiloop();
    otaloop();
	mqttloop();
	dimmerloop();

}