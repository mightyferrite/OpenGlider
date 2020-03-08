
// Open Source Glider
// John Reine
// 8/12/19 - started - created states and general workflow
#include <SPI.h>
#include <SD.h>
#include <WiFiNINA.h>
#undef max
#undef min
#include <ArduinoHttpClient.h>
#include <avr/pgmspace.h>
#include "arduino_secrets.h"
#include <Ticker.h>
#include <BME280I2C.h>
#include <Wire.h>

#define DEBUGGING
#define CALLBACK_FUNCTIONS 1

#define LED_BUILTIN 3

boolean AUTOSTART = false;
boolean WAIT_FOR_SERIAL = false;
boolean LOG_TO_FILE = true;
boolean NO_VALVE = true;
boolean NO_WEB = false;

const byte pumpPlusRelay = 8;
const byte pumpMinusRelay = 7;
const byte pumpPowerRelay = 6;
const byte valvePlusRelay = 10;
const byte valveMinusRelay = A1;
const byte valvePowerRelay = 9;
const byte buzzerOnPin = 5;          //buzzer connected

const boolean relayON = LOW;
const boolean relayOFF = HIGH;
const float version = 0.01;
const boolean ON = true;
const boolean OFF = false;
int def_buzz = 400;           //default buzz time for status updates..
boolean gotWebInterface = false;
int numberOfDives = 3;        //How many times it will dive on it's own repeatedly
String allData;

unsigned long on_surface_time = 30000;
unsigned long on_bottom_time = 30000;
unsigned long diving_time = 200000;
unsigned long surfacing_time = 200000;
unsigned long pump_surfacing_time = 180000; //3 min SURF
unsigned long pump_diving_time = 180000;    //3 min DIVE
unsigned long power_on_time = 180000;     //wait 30 seconds before deploying
unsigned long valve_on_time = 710;
unsigned long valve_on_buzzer_time = 500;
unsigned long pump_diving_buzzer_time = 1000;
unsigned long pump_surfacing_buzzer_time = 1000;

unsigned long buzzer_time_high = 800;
unsigned long buzzer_time_low = 300;

void setupSensors();
void setupWebInterface();
void loopGliderState();
void loopPumpState();
void loopValveState();
void loopBuzzerState();
void loopSensors();
void loopWebInterface();
void setupBuzzer();
int buzzer_repeat_count = 2;

enum GliderState {
  POWER_ON,
  POWER_OFF,
  ON_SURFACE,
  ON_SURFACE_WAITING_FOR_COMMAND,
  DIVING,
  ON_BOTTOM,
  SURFACING,
  LEAK
};

enum DeployState {
  DEPLOY_OFF,
  DEPLOYED,
  DEPLOYING,
  RECOVERED,
  RECOVERING
};

enum PumpState {
  PUMP_OFF_INIT,
  PUMP_OFF,
  PUMP_ON_IN_INIT,
  PUMP_ON_IN,
  PUMP_OFF_SETUP,
  PUMP_ON_OUT_INIT,
  PUMP_ON_OUT
};

enum ValveState {
  VALVE_OPEN,
  VALVE_OPEN_INIT,
  VALVE_CLOSE_INIT,
  VALVE_CLOSED
};


enum BuzzerState {
  BUZZER_HIGH,
  BUZZER_LOW,
  BUZZER_OFF
};

struct Buzzer {
  unsigned long buzzerTimer = 0;
  int buzzerFrequency = 2500;
  int buzzerLowTime = 1000000 / buzzerFrequency / 2;
  int buzzerHighTime = 0;
  int buzzerCounter = 0;
  int buzzerCount = 1;
  BuzzerState buzzerState = BUZZER_OFF;
};

struct SensorData {
  BME280I2C bme;

};

struct GliderData {
  GliderState state = POWER_ON;
  unsigned long stateTimer = 0;
  unsigned long pumpTimer = 0;
  unsigned long pumpBuzzerTimer = 0;
  unsigned long valveTimer = 0;
  unsigned long valveBuzzerTimer = 0;
  Buzzer buzzer;
  DeployState deployState = DEPLOY_OFF;
  int diveCounter = 0;
  PumpState pumpState = PUMP_OFF;
  ValveState valveState = VALVE_CLOSED;   //initially valve will be assumed closed, though we need to factor in feedback for this
};

GliderData glider;
unsigned long currentMillis = millis();

void setup() {
  Serial.begin(115200);
  if (WAIT_FOR_SERIAL) {
    int serialDelayCounter;
    while (!Serial) {
      delay(100);
      serialDelayCounter++;
      if (serialDelayCounter > 20) {
        break;
      }
      // wait for serial port to connect. Needed for native USB port only
    }
  }
//  if (Serial) {
//    //AUTOSTART = false;
//  }
  //digitalWrite(LED_BUILTIN, HIGH);
  currentMillis = millis();
  glider.stateTimer = currentMillis;

  glider.state = POWER_ON;
  glider.valveState = VALVE_CLOSED;
  glider.diveCounter = 0;
  setupSDCard();
  sdCardInfo();
  setupRelays();
  setupBuzzer();
  setupSensors();
  if (!NO_WEB) {
    setupWebInterface();
  }
}

void loop() {
  currentMillis = millis();
  loopGliderState();
  loopPumpState();
  loopValveState();
  loopBuzzerState();
  loopSensors();
  if (!NO_WEB) {
    loopWebInterface();
  }
  loopRelays();
  loopBuzzerState();
}
