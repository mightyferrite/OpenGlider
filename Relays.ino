

int b = 0;

void setupRelays() {
  pinMode(pumpPlusRelay, OUTPUT);
  pinMode(pumpMinusRelay, OUTPUT);
  pinMode(pumpPowerRelay, OUTPUT);
  pinMode(valvePlusRelay, OUTPUT);
  pinMode(valveMinusRelay, OUTPUT);
  pinMode(valvePowerRelay, OUTPUT);
  digitalWrite(valveMinusRelay, HIGH);
  digitalWrite(valvePowerRelay, HIGH);
  digitalWrite(valvePlusRelay, HIGH);
  digitalWrite(pumpPlusRelay, HIGH);
  digitalWrite(pumpMinusRelay, HIGH);
  digitalWrite(pumpPowerRelay, HIGH);


}

void loopRelays() {
  int state;
  if (WAIT_FOR_SERIAL) {
    if (Serial.available() > 0)
    {
      // read the incoming byte:
      b = Serial.read();
      if (b == 113) {     //q  open valve
        Serial.println("open valve (wait 7 seconds)");
        valveOpenInit();
        delay(7100);
        valveOpenDone();
      }
      else if (b == 119) { //w  close valve
        logln("close valve (wait 7 seconds)");
        valveCloseInit();
        delay(7100);
        valveOpenDone();
      }
      else if (b == 101) { //e   pump IN ON
        logln("pump IN");
        pumpIn();
      }
      else if (b == 114) { //r   pump OUT ON
        logln("pump OUT");
        pumpOut();
      }
      else if (b == 116) { //t   pump OFF
        logln("pump OFF");
        pumpOff();
      }
      else if (b == 49) { //1   buzzer buzz
        buzz(buzzerOnPin, 2500, 500); // buzz the buzzer on pin buzzerOnPin at 2500Hz for 500 milliseconds
      }
    }
  }




}
