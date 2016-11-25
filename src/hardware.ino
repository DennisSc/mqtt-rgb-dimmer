#include <Arduino.h>

void hardwaresetup(){
        Serial.begin(115200);
        delay(19);
        Serial.println("Hello RGB!");

        
}

void hardwareloop(){}