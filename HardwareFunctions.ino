

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
  digitalWrite(pumpPlusRelay, relayOFF);    //both in default state is one direction
  digitalWrite(pumpMinusRelay, relayOFF);
  digitalWrite(pumpPowerRelay, relayOFF);
}

void pumpOut() {
  Serial.println("pumpOut()");
  digitalWrite(pumpPlusRelay, relayON);    //both in default state is one direction
  digitalWrite(pumpMinusRelay, relayON);
  delay(5);
  digitalWrite(pumpPowerRelay, relayON);  
}

void pumpIn() {
  Serial.println("pumpIn()");
  digitalWrite(pumpPlusRelay, relayOFF);    //both in default state is one direction
  digitalWrite(pumpMinusRelay, relayOFF);
  delay(5);
  digitalWrite(pumpPowerRelay, relayON);    
}

void pumpOff() {
  Serial.println("pumpOff()");
  digitalWrite(pumpPlusRelay, relayOFF);    //both in default state is one direction
  digitalWrite(pumpMinusRelay, relayOFF);
  digitalWrite(pumpPowerRelay, relayOFF);
}

void valveOpenInit() {
  Serial.println("valveOpenInit()");
  digitalWrite(valvePlusRelay, relayOFF);    //both in default state is one direction
  digitalWrite(valveMinusRelay, relayOFF);
  delay(5);
  digitalWrite(valvePowerRelay, relayON);
  
}
void valveCloseInit() {
  Serial.println("valveCloseInit()");
  digitalWrite(valvePlusRelay, relayON);    //both in default state is one direction
  digitalWrite(valveMinusRelay, relayON);
  delay(5);
  digitalWrite(valvePowerRelay, relayON);
}
void valveOpenDone() {
  Serial.println("valveOpenDone()");
  digitalWrite(valvePlusRelay, relayOFF);    //both in default state is one direction
  digitalWrite(valveMinusRelay, relayOFF);
  digitalWrite(valvePowerRelay, relayOFF);
}
void valveCloseDone() {
  Serial.println("valveCloseDone()");
  digitalWrite(valvePlusRelay, relayOFF);    //both in default state is one direction
  digitalWrite(valveMinusRelay, relayOFF);
  digitalWrite(valvePowerRelay, relayOFF);
}

void turnBuzzerOff() {
  digitalWrite(LED_BUILTIN, HIGH);
  //Serial.println("buzzer off");
}
void turnBuzzerOn() {
   digitalWrite(LED_BUILTIN, LOW);
   //Serial.println("buzzer on");
}
