/* Necessary Libs */
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <deprecated.h>
/* END of Dependencies */

/* Constants */

constexpr auto UID_WIDTH = 10; /* Figured out by experiments */
constexpr auto PULSE_DURATION_MS = 500; /* lenght of pulse in ms after correct UID */ 

enum { MFRC522_SPI_SS, MFRC522_RST, PWR_BTN }; /* Enumerate of OUTPUT pins names */

/* Array of OUTPUT pins */
const uint8_t output_pins[] = { 
  10, /* MFRC522_SPI_SS - SlaveSelect SPI pin for MFRC522 */
  2,  /* MFRC522_RST    - Pin for resetting MFRC522 */
  5   /* PWR_BTN        - Pin that is connected as substitute of the signal from case's power button */
  };
  
const uint8_t target_uid[UID_WIDTH] = {1, 2, 3, 4, 0, 0, 0, 0, 0, 0}; /* UID of the only working card */

/* Global variables and objects */
MFRC522 reader(output_pins[MFRC522_SPI_SS], output_pins[MFRC522_RST]);

/* My Functions */
bool IsTargetUIDPresent(MFRC522 *mfrc522) {  
  /* Test whether a card is present */
  /* Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle. */
  if ( ! mfrc522->PICC_IsNewCardPresent()) {
    return false;
  }
  
  /* Select one of the cards */
  if ( ! mfrc522->PICC_ReadCardSerial()) {
    return false;
  }
  /* Properly end communication */
  mfrc522->PICC_HaltA();

  /* Compare UIDs */
  for (auto i = 0; i < sizeof(mfrc522->uid.uidByte); ++i) {
    if (mfrc522->uid.uidByte[i] != target_uid[i]) 
      return false;
  }
  
  return true; /* We found right device */
}
/* Tasks */
/* Task pointer */
void (*task)(void);
/* Task 1 - locate correct UID */
void locateCard(void) {
  if (IsTargetUIDPresent(&reader)) {
    task = makePulse; /* move to Task2 */
  }
}
/* Task 2 - make a pulse */
void makePulse(void) {
  digitalWrite(output_pins[PWR_BTN], HIGH);
  delay(PULSE_DURATION_MS);
  digitalWrite(output_pins[PWR_BTN], LOW);
  delay(PULSE_DURATION_MS);
  
  task = locateCard; /* move back to Task1 */
}
/* Init (Executed once) Code */
void setup(void) {
  /* set all OUTPUT pins as OUTPUT - MUST BE DONE AS THE FIRST IN setup() */
  for (auto pin : output_pins) 
    pinMode(pin, OUTPUT);
  
  /* init of RFID reading */
  SPI.begin();
  reader.PCD_Init(); /* start reader */
  reader.PCD_SetAntennaGain(reader.RxGain_max); /* extend Antenna's reading range */  
    
  /* output pin init state */
  digitalWrite(output_pins[PWR_BTN], LOW);
  
  /* start with the 1st task*/
  task = locateCard;
}

/* Endlessly running code */
void loop(void) {
  task();   
}
