/* Necessary Libs */
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <deprecated.h>
/* END of Dependencies */

/* Constants */

constexpr auto UID_WIDTH = 4;

enum { MFRC522_SPI_SS, MFRC522_RST, PWR_BTN }; /* Enumerate of OUTPUT pins names */

/* Array of OUTPUT pins */
const uint8_t output_pins[] = { 
  10, /* MFRC522_SPI_SS - SlaveSelect SPI pin for MFRC522 */
  2,  /* MFRC522_RST    - Pin for resetting MFRC522 */
  5   /* PWR_BTN        - Pin that is connected as substitute of the signal from case's power button */
  };
  
const uint8_t target_uid[UID_WIDTH]; /* UID of the only working card */

/* Global variables and objects */
MFRC522 reader(output_pins[MFRC522_SPI_SS], output_pins[MFRC522_RST]);

/* My Functions */
//TODO make reader local variable: void readCard(MFRC522 *reader) 
void readCard(void) {  
  uint8_t card_uid[UID_WIDTH];
  /* Test whether a card is present */
  /* Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle. */
  if ( ! reader.PICC_IsNewCardPresent()) {
    return;
  }
  
  /* Select one of the cards */
  if ( ! reader.PICC_ReadCardSerial()) {
    return;
  }
  /* Properly end communication */
  reader.PICC_HaltA();

  /* Display UID */
  for (auto uidByte : reader.uid.uidByte) {
    Serial.print(uidByte);
    Serial.print(" ");
  }
  Serial.println("");
}

/* Init (Executed once) Code */
void setup(void) {
  /* set all OUTPUT pins as OUTPUT - MUST BE DONE AS THE FIRST IN setup() */
  for (auto pin : output_pins) 
    pinMode(pin, OUTPUT);
  // DEBUG
  Serial.begin(9600);
// END OF DEBUG

  SPI.begin();
  reader.PCD_Init(); /* start reader */
  reader.PCD_SetAntennaGain(reader.RxGain_max); /* extend Antenna's reading range */  
  
  Serial.println("Setup Finished");
}

/* Endlessly running code */
void loop(void) {
  readCard();

// TODO start the PC with "PWR_BTN" pin
    
}
