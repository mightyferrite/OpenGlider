
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
  diveCounter = 0;
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

void loopGliderState() {
  if (glider.state == POWER_ON) {
    //for now we will assume that power on only happens on the surface.. later check to see if we are actually on the surface
    if (currentMillis - glider.stateTimer > power_on_time) {
      changeGliderState(ON_SURFACE);
    }
  }
  else if (glider.state == ON_SURFACE) {
    if (currentMillis - glider.stateTimer > on_surface_time) {
      if(glider.diveCount < numberOfDives) {
         changeGliderState(DIVING);
      }
      else {
        changeGliderState(ON_SURFACE_WAITING_FOR_COMMAND);
      }
    }
    
  }
  else if (glider.state == DIVING) {
    if (currentMillis - glider.stateTimer > diving_time) {
      changeGliderState(ON_BOTTOM);
    }
  }
  else if (glider.state == ON_BOTTOM) {
    if (currentMillis - glider.stateTimer > on_bottom_time) {
      changeGliderState(SURFACING);
    }
  }
  else if (glider.state == SURFACING) {
    if (currentMillis - glider.stateTimer > surfacing_time) {
      changeGliderState(ON_SURFACE);
    }
  }
}

void loopPumpState() {
  if (glider.pumpState == PUMP_OFF) {
    //verify valve is closed

  }
  else if (glider.pumpState == PUMP_OFF_INIT) {
    if (glider.valveState == VALVE_CLOSED) {
      changePumpState(PUMP_ON_IN);
    }
  }
  else if (glider.pumpState == PUMP_ON_IN_INIT) {
    if (glider.valveState == VALVE_OPEN) {
      changePumpState(PUMP_ON_IN);
    }
  }
  else if (glider.pumpState == PUMP_ON_IN) {
    if (currentMillis - glider.pumpTimer > pump_diving_time) {
      changePumpState(PUMP_OFF_INIT);
    }
  }
  else if (glider.pumpState == PUMP_ON_OUT_INIT) {

  }
  else if (glider.pumpState == PUMP_ON_OUT) {
    if (currentMillis - glider.pumpTimer > pump_surfacing_time) {
      changePumpState(PUMP_OFF_INIT);
    }
  }
  else if (glider.pumpState == PUMP_OFF_INIT) {
    changePumpState(PUMP_OFF);
  }

}

void loopValveState() {
  if (glider.valveState == VALVE_OPEN) {

  }
  else if (glider.valveState == VALVE_OPEN_INIT) {
    if (currentMillis - glider.valveTimer > valve_on_time) {
      changeValveState(VALVE_OPEN);
    }
  }
  else if (glider.valveState == VALVE_CLOSE_INIT) {
    if (currentMillis - glider.valveTimer > valve_on_time) {
      changeValveState(VALVE_CLOSED);
    }
  }
  else if (glider.valveState == VALVE_CLOSED) {

  }
}

void changeGliderState(GliderState newGliderState) {
  if (newGliderState == ON_SURFACE) {
    Serial.println("glider.state switching to ON_SURFACE");
    if (glider.state == SURFACING) {          //glider is coming up to the surface.. turn pump off..
      //reached the surface from the bottom
      glider.stateTimer = currentMillis;
      grider.diveCounter++;
    }
    else if (glider.state == POWER_ON) {
      //initial turn on!
      Serial.println("glider.state switching to POWER_ON");
      glider.stateTimer = currentMillis;

    }
  }
  else if(newGliderState == ON_SURFACE_WAITING_FOR_COMMAND) {
      Serial.println("glider.state switching to ON_SURFACE_WAITING_FOR_COMMAND");
      
  }
  else if (newGliderState == DIVING) {                      //glider is beginning a dive
    Serial.println("glider.state switching to DIVING");
    glider.stateTimer = currentMillis;
    changePumpState(PUMP_ON_IN_INIT);                       //pump water in to dive
  }
  else if (newGliderState == ON_BOTTOM) {
    Serial.println("glider.state switching to ON_BOTTOM");
    glider.stateTimer = currentMillis;
   
  }
  else if (newGliderState == SURFACING) {
    Serial.println("glider.state switching to SURFACING");
    glider.stateTimer = currentMillis;
    changePumpState(PUMP_ON_OUT_INIT);    //pump water out to surface
  }

  glider.state = newGliderState;
}

void changePumpState(PumpState newPumpState) {
  if (newPumpState == PUMP_OFF) {
    Serial.println("glider.pumpState switching to PUMP_OFF");
  }
  else if (newPumpState == PUMP_OFF_INIT) {
    Serial.println("glider.pumpState switching to PUMP_OFF_INIT");
    pumpOff();

  }
  else if (newPumpState == PUMP_ON_IN_INIT) {
    Serial.println("glider.pumpState switching to PUMP_ON_IN_INIT");
    glider.pumpTimer = currentMillis;
    Serial.println("glider.pumpState is in PUMP_ON_IN_INIT and is DIVING (this is good)");
    changeValveState(VALVE_OPEN_INIT);
    pumpInInit();
  }
  else if (newPumpState == PUMP_ON_IN) {
    Serial.println("glider.pumpState switching to PUMP_ON_IN");
  }
  else if (newPumpState == PUMP_ON_OUT_INIT) {
    Serial.println("glider.pumpState switching to PUMP_ON_OUT_INIT");
    glider.pumpTimer = currentMillis;
    changeValveState(VALVE_OPEN_INIT);
    pumpOutInit();

  }
  else if (newPumpState == PUMP_ON_OUT) {
    Serial.println("glider.pumpState switching to PUMP_ON_OUT");
  }
  else if (newPumpState == PUMP_OFF_INIT) {
    Serial.println("glider.pumpState switching to PUMP_OFF_INIT");
    glider.pumpTimer = currentMillis;
    pumpOffInit();
  }
  glider.pumpState = newPumpState;

}

void changeValveState(ValveState newValveState) {
  if (newValveState == VALVE_OPEN) {
    valveOpenDone();
  }
  else if (newValveState == VALVE_OPEN_INIT) {
    valveOpenInit();
  }
  else if (newValveState == VALVE_CLOSE_INIT) {
    valveCloseInit();
  }
  else if (newValveState == VALVE_CLOSED) {
    valveCloseDone();
  }
  glider.valveState = newValveState;
}

void pumpOutInit() {
  Serial.println("pumpOutInit()");
  //setup direction
}

void pumpInInit() {
  Serial.println("pumpInInit()");
  //setup direction
}

void pumpOffInit() {
  Serial.println("pumpOffInit()");
  //probably do nothing
}

void pumpOut() {
  Serial.println("pumpOut()");
}

void pumpIn() {
  Serial.println("pumpIn()");
}

void pumpOff() {
  Serial.println("pumpOff()");
}

void valveOpenInit() {
  Serial.println("valveOpenInit()");
}
void valveCloseInit() {
  Serial.println("valveCloseInit()");
}
void valveOpenDone() {

}
void valveCloseDone() {

}