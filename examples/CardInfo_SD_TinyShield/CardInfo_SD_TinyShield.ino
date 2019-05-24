/*
  SD card test

  This example shows how use the utility libraries on which the'
  SD library is based in order to get info about your SD card.
  Very useful for testing a card when you're not sure whether its working or not.

  The circuit:
    SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module.
 		Pin 4 used here for consistency with other Arduino examples


  created  28 Mar 2011
  by Limor Fried
  modified 9 Apr 2012
  by Tom Igoe

  modified 24 May 2019
  by Laveréna Wienclaw for TinyCircuits 
*/
// include the SD library:
#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit, and TinyCircuits SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN
const int chipSelect = 10;

// Make program compatibile for all TinyCircuits processor boards
#if defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitor SerialUSB
#else
  #define SerialMonitor Serial
#endif

void setup() {
  // Open serial communications and wait for port to open:
  SerialMonitor.begin(9600);
  while (!SerialMonitor) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  SerialMonitor.print("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    SerialMonitor.println("initialization failed. Things to check:");
    SerialMonitor.println("* is a card inserted?");
    SerialMonitor.println("* is your wiring correct?");
    SerialMonitor.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  } else {
    SerialMonitor.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  SerialMonitor.println();
  SerialMonitor.print("Card type:         ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      SerialMonitor.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      SerialMonitor.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      SerialMonitor.println("SDHC");
      break;
    default:
      SerialMonitor.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    SerialMonitor.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    while (1);
  }

  SerialMonitor.print("Clusters:          ");
  SerialMonitor.println(volume.clusterCount());
  SerialMonitor.print("Blocks x Cluster:  ");
  SerialMonitor.println(volume.blocksPerCluster());

  SerialMonitor.print("Total Blocks:      ");
  SerialMonitor.println(volume.blocksPerCluster() * volume.clusterCount());
  SerialMonitor.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  SerialMonitor.print("Volume type is:    FAT");
  SerialMonitor.println(volume.fatType(), DEC);

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  SerialMonitor.print("Volume size (Kb):  ");
  SerialMonitor.println(volumesize);
  SerialMonitor.print("Volume size (Mb):  ");
  volumesize /= 1024;
  SerialMonitor.println(volumesize);
  SerialMonitor.print("Volume size (Gb):  ");
  SerialMonitor.println((float)volumesize / 1024.0);

  SerialMonitor.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
}

void loop(void) {
}
