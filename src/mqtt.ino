#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


WiFiClient espClient;
PubSubClient client(espClient);



int getRed(String command) {
	String returnStr;
	for (int i = 0; i < 3; i++) {
		returnStr += command[i];
	}
	 
	return returnStr.toInt();
}

int getGreen(String command) {
	String returnStr;
	for (int i = 4; i < 7; i++) {
		returnStr += command[i];
	}

	return returnStr.toInt();
}

int getBlue(String command) {
	String returnStr;
	for (int i = 8; i < 11; i++) {
		returnStr += command[i];
	}

	return returnStr.toInt();
}


void callback(char* topic, byte* payload, unsigned int length) {
	
	String topic_helper;
	for (int s = 0; topic[s] != '\0'; s++)
	{
		topic_helper += topic[s];
	}

	Serial.print("Message arrived [");
	Serial.print(topic_helper);
	Serial.println("] ");



	String helper;
	helper = "";
	for (int i = 0; i < length; i++) 
	{
		helper += (char)payload[i];

	}


	if (topic_helper.substring(topic_helper.length()-2, topic_helper.length()) == "/r") 
	{
		Serial.println("topics match: rainbow");
		if (helper == "1") 
		{ 
			Serial.println("starting rainbow :)");
			FadeStepCounter = 256;
			showRainbowCycle = 1;
			doSlowCycle = 0;
		}
		else 
		{
			showRainbowCycle = 0;
			doSlowCycle = 0;
			FadeStepCounter = 0;
			Serial.println("rainbow stopped.");
			
		}
	}
	else if (topic_helper.substring(topic_helper.length() - 2, topic_helper.length()) == "/c")
	{
		Serial.println("topics match: cycle");
		if (helper == "1")
		{
			Serial.println("starting cycling :)");
			FadeStepCounter = 256;
			showRainbowCycle = 0;
			doSlowCycle = 1;
		}
		else
		{
			showRainbowCycle = 0;
			doSlowCycle = 0;
			FadeStepCounter = 0;
			Serial.println("cycling stopped.");

		}
	}
	else
	{
		showRainbowCycle = 0;
		doSlowCycle = 0;
		FadeStepCounter = 0;


		Serial.println(helper);
		
		FadeTo(getRed(helper), getGreen(helper), getBlue(helper));

	}



	
	
	
	Serial.println();



}

void reconnect() {
	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");

		// Create a random client ID
		String clientId = "ESP8266Client-";
		clientId += String(random(0xffff), HEX);
		// Attempt to connect
		if (client.connect(clientId.c_str())) {
			Serial.println("connected");


			// Once connected, publish an announcement...
			//client.publish("outTopic", "hello world");
			// ... and resubscribe
			


			client.subscribe(mqtt_topic);

			char ConcatTopic[80];
			char WildCard[5] = "/#";
			sprintf(ConcatTopic, "%s%s", mqtt_topic, WildCard);
			Serial.println(ConcatTopic);

			client.subscribe(ConcatTopic);

		}
		else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

void mqttsetup() {
	int port = atoi(mqtt_port);
	client.setServer(mqtt_server, port);
	client.setCallback(callback);
}

void mqttloop() {

	if (!client.connected()) {
		reconnect();
	}
	client.loop();


}