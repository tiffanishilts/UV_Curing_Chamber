#include "Curing_Chamber_Variables.h"

#include <Wire.h>
#include "Adafruit_MCP9808.h"

// Pin Assignments
#define RELAY 4
#define T1 16   // DM Transistor
#define T2 33   // Heating Pad Transistor
#define ME 12   // Motor Enable
#define M1 26   // Motor Pin 1
#define M2 27   // Motor Pin 2
#define PWM_CHANNEL 0 // Channel for analog writing to motor
#define MOTOR_SPEED 100 // PWM of 100/255 on motor enable pin.

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

// Number of days between reset to handle millis() overflow
#define RESET_DAYS 25

// Preset Memory Locations
#define P1_TEMP 0
#define P1_TIME 1
#define P2_TEMP 2
#define P2_TIME 3
#define P3_TEMP 4
#define P3_TIME 5
#define P4_TEMP 6
#define P4_TIME 7
#define P5_TEMP 8
#define P5_TIME 9
#define EEPROM_SIZE 10

// Global Variables
bool cureTrigger = 0;  // Has curing been initiated?
bool cureState = 0;   // Is curing in process?
bool dmStatus = 0;    // Dead Man Switch Status
bool rising = 0;
bool pauseTrigger = 0; // Pauses curing
bool pauseState = 0;
bool resumeTrigger = 0; // resume after pausing
bool cancelTrigger = 0; // Cancels curing

float cureTemp = 0;   // in C
unsigned long int cureTime = 0;   // In ms
long int cureProgress = 0; // In ms
float tempLower = 0;
float tempUpper= 0;
float currentTemp = 0; // in C


int regResin[2] = {50, 30};
int toughResin[2] = {60, 45};
int highResin[2] = {80, 60};
int preset1Resin[2] = {};
int preset2Resin[2] = {};
int preset3Resin[2] = {};
int preset4Resin[2] = {};
int preset5Resin[2] = {};

// Include Menu Code
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

  // Initalize EEPROM and preset values
  if (!EEPROM.begin(EEPROM_SIZE))
{
  Serial.println("failed to initialise EEPROM"); delay(1000000);
}
  preset1Resin[0] = EEPROM.read(P1_TEMP);
  preset1Resin[1] = EEPROM.read(P1_TIME);
  preset2Resin[0] = EEPROM.read(P2_TEMP);
  preset2Resin[1] = EEPROM.read(P2_TIME);
  preset3Resin[0] = EEPROM.read(P3_TEMP);
  preset3Resin[1] = EEPROM.read(P3_TIME);
  preset4Resin[0] = EEPROM.read(P4_TEMP);
  preset4Resin[1] = EEPROM.read(P4_TIME);
  preset5Resin[0] = EEPROM.read(P5_TEMP);
  preset5Resin[1] = EEPROM.read(P5_TIME);

  // put your setup code here, to run once:
  pinMode(RELAY, OUTPUT);
  pinMode(T1, INPUT); // Reads status of DM switch
  pinMode(T2, OUTPUT); // Controls triggering of heating pad

  //pinMode(ME, OUTPUT);
  ledcSetup(PWM_CHANNEL, 5000, 8); // Create a PWM channel '0' with freq 5000Hz, and resolution of 8
  ledcAttachPin(ME, PWM_CHANNEL);  // Attach motor enable to PWM channel.
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  digitalWrite(M1, 0);  // Enable Motor
  digitalWrite(M2, 1);
  ledcWrite(PWM_CHANNEL, 0);  // Initialize motor to be off

  digitalWrite(RELAY, 0);   // Initialize RELAY to off mode

  pinMode(ROT_A, INPUT);
  pinMode(ROT_B, INPUT);
  pinMode(BUTT_PRESS, INPUT);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  startup_screen();
  intialize_menu();
  display_menu(menu, option, 0);

}

void loop() {
  update_display();
}
