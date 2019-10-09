
Ticker sensorTimer(getSensorData, 5000, MILLIS);
BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
// Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

unsigned long BMESensorTimer = 0;
unsigned long BMESensortime = 10000;
void setupSensors() {
  Wire.begin();
  setupBME();
  //printBME280Data(&Serial);
  //logBME280Data();
}

void getSensorData() {
  //logBME280Data();
  log(getBMEData());
}

void loopSensors() {
  if (currentMillis - BMESensorTimer > BMESensortime) {
    getSensorData();
    BMESensorTimer = currentMillis;
    //allData = allData + getBMEData() + " \r\n";
  }
}

void setupBME() {
  int bmeCounter = 0;
  while (!bme.begin()) //todo - make this not a while loop!  :)
  {
    logln("Could not find BME280 sensor!");
    delay(4000);
    if(bmeCounter++ > 10){
      break;
    }
  }
  switch (bme.chipModel())
  {
    case BME280::ChipModel_BME280:
      logln("Found BME280 sensor! Success.");
      break;
    case BME280::ChipModel_BMP280:
      logln("Found BMP280 sensor! No Humidity available.");
      break;
    default:
      logln("Found UNKNOWN sensor! Error!");
  }
}

void logBME280Data()
{
  float temp(NAN), hum(NAN), pres(NAN), tempf(NAN);
  // PresUnit_Pa,  PresUnit_hPa, PresUnit_inHg, PresUnit_atm, PresUnit_bar, PresUnit_torr, PresUnit_psi
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  //BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  BME280::PresUnit presUnit(BME280::PresUnit_psi);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  log("Temp: ");
  log(temp);
  String s1 = "°" + String(tempUnit == BME280::TempUnit_Celsius ? 'C' : 'F');
//  int slength = s1.length() + 1;
//  char char_array[slength];
//  s1.toCharArray(char_array, slength);
  log(s1);
  log("\tHumidity: ");
  log(hum);
  log("% RH");
  log("\tPressure: ");
  log(pres);
  logln(" Psi");
  log("temp=");
  tempf = temp * 1.8 + 32;
  log(tempf);
  logln(" F");
  
}
void printBME280Data(Stream* client)
{
  float temp(NAN), hum(NAN), pres(NAN), tempf(NAN);
  // PresUnit_Pa,  PresUnit_hPa, PresUnit_inHg, PresUnit_atm, PresUnit_bar, PresUnit_torr, PresUnit_psi
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  //BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  BME280::PresUnit presUnit(BME280::PresUnit_psi);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  client->print("Temp: ");
  client->print(temp);
  client->print("°" + String(tempUnit == BME280::TempUnit_Celsius ? 'C' : 'F'));
  client->print("\tHumidity: ");
  client->print(hum);
  client->print("% RH");
  client->print("\tPressure: ");
  client->print(pres);
  client->println(" Psi");
  client->print("temp=");
  tempf = temp * 1.8 + 32;
  client->print(tempf);
  client->println(" F");
  //client->println(" Pa");
  //delay(1000);
}

String getBMEData() {
  String returnString;
  float temp(NAN), hum(NAN), pres(NAN), tempf(NAN);
  // PresUnit_Pa,  PresUnit_hPa, PresUnit_inHg, PresUnit_atm, PresUnit_bar, PresUnit_torr, PresUnit_psi
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  //BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  BME280::PresUnit presUnit(BME280::PresUnit_psi);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  returnString = ("");
  returnString = returnString + (temp);
  returnString = returnString +(String(tempUnit == BME280::TempUnit_Celsius ? 'C' : 'F'));
  returnString = returnString +(",");
  returnString = returnString +(hum);
  returnString = returnString +(",");
  //returnString = returnString +("\t");
  returnString = returnString +(pres);
  //returnString = returnString +(" Psi");
  returnString = returnString +(",");
  tempf = temp * 1.8 + 32;
  //returnString = returnString +(tempf);
  returnString = returnString + "\n";
  //returnString = returnString +(" F");
  //client->println(" Pa");
  //delay(1000);
  return returnString;
}
