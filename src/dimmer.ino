#include <Arduino.h>
#include <FastLED.h>


CRGBArray<NUM_LEDS> leds;

uint16_t hue = 0;


void dimmersetup() {
	FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
	showRainbowCycle = 0;
	
	R_now = 255;
	G_now = 0;
	B_now = 0;



	for (int xyz = 0; xyz < 499; xyz++) {
		leds.fill_rainbow(hue++);
		FastLED.delay(20);
		FastLED.show();
	}

}

//initialize and calculate new fades
void FadeTo(int red, int green, int blue)
{
	

	
	FadeStepCounter = 0;

	//current values become "old" values > current values will be changed, so we store the initial values somewhere else for later use
	R_old = R_now;
	G_old = G_now;
	B_old = B_now;

	R_target = red;
	G_target = green;
	B_target = blue;




	//calculate differences between old and target values, so we know how much to add with each step
	int R_diff = R_target - R_old;
	R_Add = ((float)R_diff / FADESTEPS);

	int G_diff = G_target - G_old;
	G_Add = ((float)G_diff / FADESTEPS);

	int B_diff = B_target - B_old;
	B_Add = ((float)B_diff / FADESTEPS);



	// /*debug
	Serial.print("R_old: ");
	Serial.println(R_old);
	Serial.print("R_target: ");
	Serial.println(R_target);
	Serial.print("R_diff: ");
	Serial.println(R_diff);
	Serial.print("R_Add: ");
	Serial.println(R_Add);


	Serial.print("G_old: ");
	Serial.println(G_old);
	Serial.print("G_target: ");
	Serial.println(G_target);
	Serial.print("G_diff: ");
	Serial.println(G_diff);
	Serial.print("G_Add: ");
	Serial.println(G_Add);

	Serial.print("B_old: ");
	Serial.println(B_old);
	Serial.print("B_target: ");
	Serial.println(B_target);
	Serial.print("B_diff: ");
	Serial.println(B_diff);
	Serial.print("B_Add: ");
	Serial.println(B_Add);

	//*/


	Serial.println("starting fade process");

}

void rainbowCycle()
{
	leds.fill_rainbow(hue++);
	FastLED.delay(20);
	FastLED.show();
}

void slowCycleAllLeds()
{
	for (int i = 0; i < NUM_LEDS; i++) {
		// let's set a led value
		leds[i] = CHSV(hue, 255, 255);


	}
	hue++;
	FastLED.delay(100);
	FastLED.show();
}


//actual fading bewteen colors happens here!
void dimmerloop() {

	// mqtt rainbow cycle command received, setting up for rainbowCycle
	if (showRainbowCycle)
	{
		doSlowCycle = 0;
		rainbowCycle();
	} 

	// mqtt slow cycle command received, setting up for SlowCycle
	else if (doSlowCycle)
	{
		showRainbowCycle = 0;
		slowCycleAllLeds();
	} 

	// mqtt normal color fade operation received...
	else
	{

		while (FadeStepCounter < FADESTEPS)
		{
			FadeStepCounter++;

			R_now = R_old + (FadeStepCounter * R_Add);
			G_now = G_old + (FadeStepCounter * G_Add);
			B_now = B_old + (FadeStepCounter * B_Add);

		
			Serial.print("RGB value: ");
			Serial.print(R_now);
			Serial.print(";");
			Serial.print(G_now);
			Serial.print(";");
			Serial.println(B_now);

			for (int i = 0; i < NUM_LEDS; i++) {
				// let's set an led value
				//leds[i] = CHSV(hue++, 255, 255);
				leds[i] = CRGB(R_now, G_now, B_now);
			

			}
			//leds.fill_rainbow(hue++);
			FastLED.delay(FADECLOCK);
			FastLED.show();
		}

		while (FadeStepCounter == FADESTEPS) 
		{
			Serial.println("Finished.");
			FadeStepCounter++;
		}
	} //end else

	

	
}