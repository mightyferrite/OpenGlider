// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume sdvolume;
SdFile root;
void logToWebClientSSE(String tolog);
const int chipSelect = 4;

void log(String msg) {
  if (WAIT_FOR_SERIAL) {
    Serial.print(msg);
  }
  if (LOG_TO_FILE) {
    File dataFile;
    dataFile = SD.open("9_25_19.txt", FILE_WRITE);
    dataFile.print(msg);
    dataFile.close();
  }
  logToWebClientSSE(msg);
}
void logln(String msg) {
  if (WAIT_FOR_SERIAL) {
    Serial.println(msg);
  }
  //logToWebClientSSE(msg);
  if (LOG_TO_FILE) {
    File dataFile;
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    dataFile.println(msg);
    dataFile.close();
  }

}
void log(const char *msg) {
  if (WAIT_FOR_SERIAL) {
    Serial.print(msg);
  }
  if (LOG_TO_FILE) {
    File dataFile;
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    dataFile.print(msg);
    dataFile.close();
  }
  logToWebClientSSE(msg);
}

void logln(const char *msg) {
  if (WAIT_FOR_SERIAL) {
    Serial.println(msg);
  }
  if (LOG_TO_FILE) {
    File dataFile;
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    dataFile.println(msg);
    dataFile.close();
  }
}
void setupSDCard() {
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    logln("initialization failed. Things to check:");
    logln("* is a card inserted?");
    logln("* is your wiring correct?");
    logln("* did you change the chipSelect pin to match your shield or module?");
    while (1);  //TODO remove this... will run without SD card.. JTR
  } else {
    logln("Wiring is correct and a card is present.");
  }

  // print the type of card
  Serial.println();
  Serial.print("Card type:         ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }
}
void sdCardInfo() {
  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!sdvolume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    //while (1);
  }

  Serial.print("Clusters:          ");
  Serial.println(sdvolume.clusterCount());
  Serial.print("Blocks x Cluster:  ");
  Serial.println(sdvolume.blocksPerCluster());

  Serial.print("Total Blocks:      ");
  Serial.println(sdvolume.blocksPerCluster() * sdvolume.clusterCount());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("Volume type is:    FAT");
  Serial.println(sdvolume.fatType(), DEC);

  volumesize = sdvolume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= sdvolume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  Serial.print("Volume size (Kb):  ");
  Serial.println(volumesize);
  Serial.print("Volume size (Mb):  ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Gb):  ");
  Serial.println((float)volumesize / 1024.0);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  //  root.openRoot(volume);
  //
  //  // list all files in the card with date and size
  //  root.ls(LS_R | LS_DATE | LS_SIZE);
  //  root.close();
  //
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  if (LOG_TO_FILE) {
    File dataFile;

    dataFile = SD.open("datalog.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      log("Log file for Deployment 1\n" );
      dataFile.close();
      // print to the serial port too:
      //Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
  }

}
