
void setupBuzzer() {
  pinMode(buzzerOnPin, OUTPUT); // set a pin for buzzer output
  buzz(buzzerOnPin, 3500, 500);
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
  digitalWrite(pumpPlusRelay, relayOFF);    //both in default state is one direction
  digitalWrite(pumpMinusRelay, relayOFF);
  digitalWrite(pumpPowerRelay, relayOFF);
}

void pumpOut() {
  Serial.println("pumpOut()");
  digitalWrite(pumpPlusRelay, relayOFF);    //both in default state is one direction
  digitalWrite(pumpMinusRelay, relayOFF);
  delay(5);
  digitalWrite(pumpPowerRelay, relayON);
}

void pumpIn() {
  Serial.println("pumpIn()");
  digitalWrite(pumpPlusRelay, relayON);    //both in default state is one direction
  digitalWrite(pumpMinusRelay, relayON);
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
  digitalWrite(valvePlusRelay, relayON);    //both in default state is one direction
  digitalWrite(valveMinusRelay, relayON);
  delay(5);
  digitalWrite(valvePowerRelay, relayON);

}
void valveCloseInit() {
  Serial.println("valveCloseInit()");
  digitalWrite(valvePlusRelay, relayOFF);    //both in default state is one direction
  digitalWrite(valveMinusRelay, relayOFF);
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
  digitalWrite(buzzerOnPin, HIGH);
  //Serial.println("buzzer off");
}
void turnBuzzerOn() {
  
  //Serial.println("buzzer on");
}

void buzzerStateChange(int newState){
  if(glider.buzzer.buzzerState == BUZZER_HIGH){
    digitalWrite(LED_BUILTIN, HIGH); 
  } else if(glider.buzzer.buzzerState == BUZZER_LOW){
    digitalWrite(LED_BUILTIN, LOW); 
  }
  else if(glider.buzzer.buzzerState == BUZZER_OFF){
    digitalWrite(LED_BUILTIN, LOW); 
  }
  
}


void buzz(int targetPin, long frequency, long length) {
  Serial.println("buzz start");
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait againf or the calculated delay value
  }
  Serial.println("buzz ended");
}
