

const int r1 = 6; //d-io 0-1 are tx/rx
const int r2 = 7;
const int r3 = 8;
const int r4 = 9;
const int r5 = 9;
const int r6 = 9;
const int r7 = 9;
const int r8 = 9;
int incomingByte = 0;

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

  
  pinMode (r1, OUTPUT);
  pinMode (r2, OUTPUT);
  pinMode (r3, OUTPUT);
  pinMode (r4, OUTPUT);
  pinMode (r5, OUTPUT);
  pinMode (r6, OUTPUT);
  pinMode (r7, OUTPUT);
  pinMode (r8, OUTPUT);

  digitalWrite(r1, HIGH); //default all relays off
  digitalWrite(r2, HIGH);
  digitalWrite(r3, HIGH);
  digitalWrite(r4, HIGH);
  digitalWrite(r5, HIGH);
  digitalWrite(r6, HIGH);
  digitalWrite(r7, HIGH);
  digitalWrite(r8, HIGH);

}

void loopRelays() {
  int state;

  if (Serial.available() > 0)
  {
    // read the incoming byte:
    incomingByte = Serial.read();
    //Serial.println("received data");
  }

  if (incomingByte >= 49)
  {
    if (incomingByte <= 56)
    {
      state = incomingByte - 48;
      if (state == 1)
      {
        digitalWrite(r1, HIGH);
      }
      else if (state == 2)
      {
        digitalWrite(r2, HIGH);
      }
      else if (state == 3)
      {
        digitalWrite(r3, HIGH);
      }
      else if (state == 4)
      {
        digitalWrite(r4, HIGH);
      }
      else if (state == 5)
      {
        digitalWrite(r5, HIGH);
      }
      else if (state == 6)
      {
        digitalWrite(r6, HIGH);
      }
      else if (state == 7)
      {
        digitalWrite(r7, HIGH);
      }
      else if (state == 8)
      {
        digitalWrite(r8, HIGH);
      }
      //delay(2000);
      Serial.print("I received: ");
      Serial.println(state);
      //delay(6000);
      incomingByte = 0;
    }
  }
  if (incomingByte == 43) // + as turn on all pins
  {
    digitalWrite(r1, LOW);
    digitalWrite(r2, LOW);
    digitalWrite(r3, LOW);
    digitalWrite(r4, LOW);
    digitalWrite(r5, LOW);
    digitalWrite(r6, LOW);
    digitalWrite(r7, LOW);
    digitalWrite(r8, LOW);
  }
  if (incomingByte == 45) // - as turn off all pins
  {
    digitalWrite(r1, HIGH);
    digitalWrite(r2, HIGH);
    digitalWrite(r3, HIGH);
    digitalWrite(r4, HIGH);
    digitalWrite(r5, HIGH);
    digitalWrite(r6, HIGH);
    digitalWrite(r7, HIGH);
    digitalWrite(r8, HIGH);
  }

  if (incomingByte == 97) // a = on
  {
    digitalWrite(r1, LOW);
  }
  if (incomingByte == 98) // b = on
  {
    digitalWrite(r2, LOW);
  }
  if (incomingByte == 99) // c = on
  {
    digitalWrite(r3, LOW);
  }
  if (incomingByte == 100) // d = on
  {
    digitalWrite(r4, LOW);
  }
  if (incomingByte == 101) // e = on
  {
    digitalWrite(r5, LOW);
  }
  if (incomingByte == 102) // f = on
  {
    digitalWrite(r6, LOW);
  }
  if (incomingByte == 103) // g = on
  {
    digitalWrite(r7, LOW);
  }
  if (incomingByte == 104) // h = on
  {
    digitalWrite(r8, LOW);
  }

  if (incomingByte <= 104)
  {
    if (incomingByte >= 97)
    {
      Serial.print("I received: ");
      Serial.println(incomingByte);
      incomingByte = 0;
    }
  }

  //delay(100);
}
