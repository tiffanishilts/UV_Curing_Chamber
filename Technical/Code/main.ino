// Pin Assignments
#define RELAY 4
#define T1 16
#define T2 33

// ESP32 Board TFT I2C Interface Pinout
#define TFT_CS        5
#define TFT_SCLK      18
#define TFT_MOSI      23
#define TFT_DC        19
#define TFT_RST       17

// GPIO PINS
#define ROT_A 32
#define ROT_B 35
#define BUTT_PRESS 34

// Global Variables
bool cureTrigger;  // Has curing been initiated?
bool cureState;   // Is curing in process?
bool dmStatus;    // Dead Man Switch Status
float cureTemp;   // in C
unsigned long int cureTime;   // In ms
long int cureProgress; // In ms
float tempLower;
float tempUpper;
float currentTemp; // in C
bool rising;

int regResin[2] = {50, 30};
int toughResin[2] = {60, 45};
int highResin[2] = 

#include <Wire.h>
#include "Adafruit_MCP9808.h"
#include "Curing_Chamber_Variables.h"

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

void setup() {

  Serial.begin(9600);

  if (!tempsensor.begin())
    {
    Serial.println("Couldn't find MCP9808!");
    while (1);
    }

  // put your setup code here, to run once:
  pinMode(RELAY, OUTPUT);
  pinMode(T1, INPUT); // Reads status of DM switch
  pinMode(T2, OUTPUT); // Controls triggering of heating pad

  pinMode(ROT_A, INPUT);
  pinMode(ROT_B, INPUT);
  pinMode(BUTT_PRESS, INPUT);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  startup_screen();
  intialize_menu();
  display_menu(menu, option);

// Initialize Variables
  dmStatus = 0;
  cureState = 0;
  cureTemp = 0;
  cureTime = 0;
  rising = 0;
}

void loop() {

  currentTemp = tempsensor.readTempC();
  if ( (millis()%5000) < 1) {
    Serial.println(currentTemp);
  }

  update_display();

// Handles the initialization of the curing process
if (cureTrigger == 1) {   // if curing has been triggered, set up the cure time and set curing state
    cureProgress = cureTime;
    cureTime += millis();
    tempLower = cureTemp - 5;
    tempUpper = cureTemp + 5;
    cureTrigger = 0;
    rising = 1;
    cureState = 1;
    digitalWrite(RELAY, 1);
  }

// Handles the whole curing process itself
if (cureState) {
  if (cureProgress < 0) {   // If the remaining cure time is less than 0
    digitalWrite(RELAY, 0);
    cureState = 0;
  } else {

    // Reads the deadman's switch
    dmStatus = digitalRead(T1);

    // Code to handle dead-man switch
    if (dmStatus = 1) {      // If the door is open, keep adding the current clock time to the time remaining to save the time.
      cureTime = cureProgress + millis();
    } else {
      cureProgress = cureTime - millis();   // If door is closed decrement cureProgress by how many millis have passed.
    }

    // Code to handle feedback switching on and off of the heating pad transistor
    if (currentTemp > tempUpper && rising) {
      rising = 0;
      digitalWrite(T2, 0);
    } else if (currentTemp < tempLower && !rising) {
      rising = 1;
      digitalWrite(T2, 1);
    }
  }

}

}
