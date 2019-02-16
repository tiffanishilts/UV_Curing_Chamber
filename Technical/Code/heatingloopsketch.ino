#include <Wire.h>
#include "Adafruit_MCP9808.h"
#define RELAY 5
#define FAN 27
#define SWITCH 34

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

  int rising = 1;
  int heating = 0;
  int tempupper = 26;
  int templower = 24;
  float tempC;

void setup() {

  Serial.begin(9600);

  if (!tempsensor.begin())
    {
    Serial.println("Couldn't find MCP9808!");
    while (1);
    }

  // put your setup code here, to run once:
  pinMode(RELAY, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(SWITCH, INPUT);
  digitalWrite(RELAY, LOW);

}

void loop() {
  if (digitalRead(SWITCH)) {

    // Read Temperature
    tempC = tempsensor.readTempC();

    if (!heating && rising) {
      digitalWrite(RELAY, HIGH);
      heating = 1;
      rising = 1;
    }

    // put your main code here, to run repeatedly:
  if ((tempC > tempupper) && rising) {
    rising = 0;
    heating = 0;
   digitalWrite(RELAY, LOW);
  } else if ( (tempC < templower) && !rising) {
    rising = 1;
    heating = 1;
   digitalWrite(RELAY, HIGH);
  }

    // Turn on Fan
    digitalWrite(FAN, HIGH);


    Serial.print("Temp: ");
    Serial.print(tempC);
    Serial.print(" C\t");
    Serial.print("Rising: ");
    Serial.print(rising);
    Serial.print("\n");
    delay(250);
 }
  else {
   digitalWrite(FAN, LOW);
   digitalWrite(RELAY, LOW);
   heating = 0;
   rising = 1;
  }
}
