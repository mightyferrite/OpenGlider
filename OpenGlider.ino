
// Open Source Glider
// John Reine
// 8/12/19 - started - created states and general workflow

const float version = 0.01;
const boolean ON = true;
const boolean OFF = false;

int numberOfDives = 1;
unsigned long pump_up_time = 10000;
unsigned long pump_dn_time = 10000;
unsigned long on_surface_time = 10000;
unsigned long on_bottom_time = 10000;
unsigned long diving_time = 10000;
unsigned long surfacing_time = 10000;
unsigned long pump_diving_time = 5000;
unsigned long pump_surfacing_time = 5000;
unsigned long power_on_time = 3000;
unsigned long valve_on_time = 2000;


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

struct GliderData {
  GliderState state = POWER_ON;
  unsigned long stateTimer = 0;
  unsigned long pumpTimer = 0;
  int diveCounter = 0;
  PumpState pumpState = PUMP_OFF;

  ValveState valveState = VALVE_CLOSED;   //initially valve will be assumed closed, though we need to factor in feedback for this
  unsigned long valveTimer = 0;
};

GliderData glider;
unsigned long currentMillis = millis();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
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
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  currentMillis = millis();
  loopGliderState();
  loopPumpState();
  loopValveState();

}
