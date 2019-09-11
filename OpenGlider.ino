
// Open Source Glider
// John Reine
// 8/12/19 - started - created states and general workflow
#undef max
#undef min
#include <SPI.h>
#undef max
#undef min
#include <SD.h>
#undef max
#undef min
#include <WiFiNINA.h>
#undef max
#undef min
#include <ArduinoHttpClient.h> 
#undef max
#undef min
#include <avr/pgmspace.h>
#undef max
#undef min
#include "arduino_secrets.h" 
#include <Ticker.h>
#undef max
#undef min
#include <BME280I2C.h>
#undef max
#undef min
#include <Wire.h>
#undef max
#undef min


#define DEBUGGING
#define CALLBACK_FUNCTIONS 1
#undef max
#undef min
//#include <WebSocketServer.h>
#undef max
#undef min
#include <WebSocketsServer.h>
#undef max
#undef min
#define LED_BUILTIN 3

boolean AUTOSTART = false;
boolean WAIT_FOR_SERIAL = true;
boolean LOG_TO_FILE = true;

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
int numberOfDives = 1;        //How many times it will dive on it's own repeatedly
String allData;

unsigned long pump_up_time = 10000;
unsigned long pump_dn_time = 10000;
unsigned long on_surface_time = 60000;
unsigned long on_bottom_time = 60000;
unsigned long diving_time = 200000;
unsigned long surfacing_time = 200000;
unsigned long pump_surfacing_time = 180000; //3 min SURF
unsigned long pump_diving_time = 180000;    //3 min DIVE
unsigned long power_on_time = 60000;     //wait 1 minute before deploying
unsigned long valve_on_time = 7100;
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
  int diveCounter = 0;
  PumpState pumpState = PUMP_OFF;
  ValveState valveState = VALVE_CLOSED;   //initially valve will be assumed closed, though we need to factor in feedback for this
};

GliderData glider;
unsigned long currentMillis = millis();

void setup() {
  Serial.begin(115200);
  
  
  if(WAIT_FOR_SERIAL){
    int serialDelayCounter;
    while (!Serial) {
      delay(100);
      serialDelayCounter++;
      if(serialDelayCounter > 20){
        break;
      }
      // wait for serial port to connect. Needed for native USB port only
    }
  }
  if(Serial){
    AUTOSTART = false;
  }
  digitalWrite(LED_BUILTIN, HIGH);
  currentMillis = millis();
  glider.stateTimer = currentMillis;
 
  glider.state = POWER_ON;
  glider.valveState = VALVE_CLOSED;
  glider.diveCounter = 0;
  //init sd card writer
  //init relays
  //init leak detect
  //init pressure/humidity/temperature sensor
  //delay(500);
  setupSDCard();
  sdCardInfo();
  //setupSensors();
  setupRelays();
  setupBuzzer();
  setupSensors();
  setupWebInterface();
}

void loop() {
  currentMillis = millis();
  loopGliderState();
  loopPumpState();
  loopValveState();
  loopBuzzerState();
  loopSensors();
  loopWebInterface();
  loopRelays();
  loopBuzzerState();
}
