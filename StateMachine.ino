
void deployGlider(){
  if(glider.state == ON_SURFACE_WAITING_FOR_COMMAND){
    changeGliderState(ON_SURFACE);
  }
}

void resetGlider(){
  changeGliderState(ON_SURFACE_WAITING_FOR_COMMAND);
}

void changeGliderState(GliderState newGliderState) {
  if (newGliderState == POWER_ON) {
    
  } else if (newGliderState == ON_SURFACE) {
    Serial.println("glider.state switching to ON_SURFACE");
    allData = allData + "\nON_SURFACE" + " \r\n";

    if (glider.state == SURFACING) {          //glider is coming up to the surface.. turn pump off..
      //reached the surface from the bottom
      glider.stateTimer = currentMillis;
      glider.diveCounter++;
    }
    else if (glider.state == POWER_ON || glider.state == ON_SURFACE_WAITING_FOR_COMMAND) {
      //initial turn on!
      glider.stateTimer = currentMillis;
      setBuzzerPeriod(def_buzz, def_buzz, 2);
    }
  }
  else if (newGliderState == ON_SURFACE_WAITING_FOR_COMMAND) {
    Serial.println("glider.state switching to ON_SURFACE_WAITING_FOR_COMMAND");
          allData = allData + "ON_SURFACE_WAITING_FOR_COMMAND" + " \r\n";

  }
  else if (newGliderState == DIVING) {                      //glider is beginning a dive
    Serial.println("glider.state switching to DIVING");
    allData = allData + "DIVING" + " \r\n";
    glider.stateTimer = currentMillis;
    changePumpState(PUMP_ON_IN_INIT);                       //pump water in to dive
    setBuzzerPeriod(def_buzz, def_buzz, 3);
  }
  else if (newGliderState == ON_BOTTOM) {
    allData = allData + "ON_BOTTOM" + " \r\n";
    Serial.println("glider.state switching to ON_BOTTOM");
    glider.stateTimer = currentMillis;
    setBuzzerPeriod(def_buzz, def_buzz, 4);
  }
  else if (newGliderState == SURFACING) {
    allData = allData + "SURFACING" + " \r\n";
    Serial.println("glider.state switching to SURFACING");
    glider.stateTimer = currentMillis;
    changePumpState(PUMP_ON_OUT_INIT);    //pump water out to surface
    setBuzzerPeriod(def_buzz, def_buzz, 5);
  }

  glider.state = newGliderState;
}

void loopGliderState() {
  if (glider.state == POWER_ON) {
    //for now we will assume that power on only happens on the surface.. later check to see if we are actually on the surface
    if (currentMillis - glider.stateTimer > power_on_time) {
      if(AUTOSTART){
        changeGliderState(ON_SURFACE);      
      }
      else {
        changeGliderState(ON_SURFACE_WAITING_FOR_COMMAND);
      }
    }
  }
  else if (glider.state == ON_SURFACE) {
    if (currentMillis - glider.stateTimer > on_surface_time) {
      if (glider.diveCounter < numberOfDives) {
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

void changePumpState(PumpState newPumpState) {
  if (newPumpState == PUMP_OFF) {
    Serial.println("glider.pumpState switching to PUMP_OFF");
    allData = allData + "PUMP_OFF" + " \r\n";
  }
  else if (newPumpState == PUMP_OFF_INIT) {
    Serial.println("glider.pumpState switching to PUMP_OFF_INIT");
    allData = allData + "PUMP_OFF_INIT" + " \r\n";
    glider.pumpTimer = currentMillis;
    changeValveState(VALVE_CLOSE_INIT);
    pumpOff();

  }
  else if (newPumpState == PUMP_ON_IN_INIT) {
    glider.pumpTimer = currentMillis;
    allData = allData + "PUMP_ON_IN_INIT" + " \r\n";
    Serial.println("glider.pumpState is in PUMP_ON_IN_INIT and is DIVING (this is good)");
    changeValveState(VALVE_OPEN_INIT);
    pumpInInit();
  }
  else if (newPumpState == PUMP_ON_IN) {
    allData = allData + "PUMP_ON_IN" + " \r\n";
    Serial.println("glider.pumpState switching to PUMP_ON_IN");
    pumpIn();
  }
  else if (newPumpState == PUMP_ON_OUT_INIT) {
    allData = allData + "PUMP_ON_OUT_INIT" + " \r\n";
    Serial.println("glider.pumpState switching to PUMP_ON_OUT_INIT");
    glider.pumpTimer = currentMillis;
    changeValveState(VALVE_OPEN_INIT);
    pumpOutInit();

  }
  else if (newPumpState == PUMP_ON_OUT) {
    allData = allData + "PUMP_ON_OUT" + " \r\n";
    Serial.println("glider.pumpState switching to PUMP_ON_OUT");
    pumpOut();
  }

  glider.pumpState = newPumpState;

}

void loopPumpState() {
  if (glider.pumpState == PUMP_OFF) {
    //verify valve is closed

  }
  else if (glider.pumpState == PUMP_OFF_INIT) {
    if (glider.valveState == VALVE_CLOSED) {
      changePumpState(PUMP_OFF);
    }
  }
  else if (glider.pumpState == PUMP_ON_IN_INIT) {
    if (glider.valveState == VALVE_OPEN) {
      changePumpState(PUMP_ON_IN);
    }
  }
  else if (glider.pumpState == PUMP_ON_IN) {
    if (currentMillis - glider.pumpTimer > pump_diving_time + valve_on_time) {
      changePumpState(PUMP_OFF_INIT);
    }
    else if (currentMillis - glider.pumpBuzzerTimer > pump_diving_buzzer_time) {
      setBuzzerPeriod(def_buzz,def_buzz,2);
      glider.pumpBuzzerTimer = currentMillis;
    }
  }
  else if (glider.pumpState == PUMP_ON_OUT_INIT) {
    if (glider.valveState == VALVE_OPEN) {
      changePumpState(PUMP_ON_OUT);
    }
  }
  else if (glider.pumpState == PUMP_ON_OUT) {
    if (currentMillis - glider.pumpTimer > pump_surfacing_time + valve_on_time) {
      changePumpState(PUMP_OFF_INIT);
    }
    else if (currentMillis - glider.pumpBuzzerTimer > pump_surfacing_buzzer_time) {
      setBuzzerPeriod(def_buzz,def_buzz,3);
      glider.pumpBuzzerTimer = currentMillis;
    }
  }
  else if (glider.pumpState == PUMP_OFF_INIT) {
    changePumpState(PUMP_OFF);
    
  }

}

void changeValveState(ValveState newValveState) {
  if (newValveState == VALVE_OPEN) {
    allData = allData + "VALVE_OPEN\n";
    valveOpenDone();
  }
  else if (newValveState == VALVE_OPEN_INIT) {
    allData = allData + "VALVE_OPEN_INIT"+ " \r\n";
    glider.valveBuzzerTimer = currentMillis;
    glider.valveTimer = currentMillis;
    valveOpenInit();
  }
  else if (newValveState == VALVE_CLOSE_INIT) {
    allData = allData + "VALVE_CLOSE_INIT"+ " \r\n";
    glider.valveBuzzerTimer = currentMillis;
    glider.valveTimer = currentMillis;
    valveCloseInit();
  }
  else if (newValveState == VALVE_CLOSED) {
    allData = allData + "VALVE_CLOSED" + " \r\n";
    valveCloseDone();
  }
  glider.valveState = newValveState;
}

void loopValveState() {
  if (glider.valveState == VALVE_OPEN) {

  }
  else if (glider.valveState == VALVE_OPEN_INIT) {
    if (currentMillis - glider.valveTimer > valve_on_time) {
      changeValveState(VALVE_OPEN);
    }
    else if (currentMillis - glider.valveBuzzerTimer > valve_on_buzzer_time) {
      setBuzzerPeriod(def_buzz+100,def_buzz,1);
      glider.valveBuzzerTimer = currentMillis;
    }
  }
  else if (glider.valveState == VALVE_CLOSE_INIT) {
    if (currentMillis - glider.valveTimer > valve_on_time) {
      changeValveState(VALVE_CLOSED);
    }
    else if (currentMillis - glider.valveBuzzerTimer > valve_on_buzzer_time) {
      setBuzzerPeriod(def_buzz+100,def_buzz,2);
      glider.valveBuzzerTimer = currentMillis;
    }
  }
  else if (glider.valveState == VALVE_CLOSED) {

  }
}

void setBuzzerPeriod(int high, int low, int repeat) {
  buzzer_time_high = high;
  buzzer_time_low = low;
  buzzer_repeat_count = repeat;
  //if (glider.buzzer.buzzerState == BUZZER_OFF) {
  glider.buzzer.buzzerCounter = 0;
  changeBuzzerState(BUZZER_HIGH);
  
  //}
}

void changeBuzzerState(BuzzerState newBuzzerState) {
  if (glider.buzzer.buzzerState == BUZZER_OFF) {
    turnBuzzerOff();
  }
  else if (glider.buzzer.buzzerState == BUZZER_HIGH) {
    glider.buzzer.buzzerTimer = currentMillis;
    turnBuzzerOn();
  }
  else if (glider.buzzer.buzzerState == BUZZER_LOW) {
    glider.buzzer.buzzerTimer = currentMillis;
    turnBuzzerOff();
  }
  glider.buzzer.buzzerState = newBuzzerState;
}

void loopBuzzerState() {
  if (glider.buzzer.buzzerState == BUZZER_OFF) {
    
  }
  else if (glider.buzzer.buzzerState == BUZZER_HIGH) {
    if (glider.buzzer.buzzerCounter <= buzzer_repeat_count) {
      if (currentMillis - glider.buzzer.buzzerTimer > buzzer_time_high) {
        changeBuzzerState(BUZZER_LOW);
        glider.buzzer.buzzerCounter++;
      }
    }
    else {
      changeBuzzerState(BUZZER_OFF);
    }
  }
  else if (glider.buzzer.buzzerState == BUZZER_LOW) {
    if (currentMillis - glider.buzzer.buzzerTimer > buzzer_time_low) {
      changeBuzzerState(BUZZER_HIGH);
    }
  }
}
