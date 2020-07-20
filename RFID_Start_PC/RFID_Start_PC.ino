/* Necessary Libs */
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <deprecated.h>
/* END of Dependencies */

/* Constants */

constexpr auto UID_WIDTH = 4;

enum { MFRC522_SPI_SS, MFRC522_RST, PWR_BTN }; /* Enumerate of OUTPUT pins name */

/* Array of OUTPUT pins */
const uint8_t output_pins[] = { 
  3, /* MFRC522_SPI_SS - SlaveSelect SPI pin for MFRC522 */
  4, /* MFRC522_RST    - Pin for resetting MFRC522 */
  5  /* PWR_BTN        - Pin that is connected as substitute of the signal from case's power button */
  };
  
const uint8_t target_uid[UID_WIDTH]; /* UID of the only working card */

/* Global variables and objects */
MFRC522 reader(output_pins[MFRC522_SPI_SS], output_pins[MFRC522_RST]);

/* Init (Executed once) Code */
void setup(void) {
  reader.PCD_Init(); /* start reader */
  //reader.PCD_SetAntennaGain(reader.RxGain_max); /* extend Antenna's reading range */

// DEBUG
  Serial.begin(9600);
// END OF DEBUG

  /* set all OUTPUT pins as OUTPUT */
  for (auto pin : output_pins) 
    pinMode(pin, OUTPUT);
}

/* Endlessly running code */
void loop(void) {  
  /* Test whether a card is present */
  if (reader.PICC_IsNewCardPresent()) {
      uint8_t card_uid[UID_WIDTH];
      
      /* Read card UID */
      if (!reader.PICC_ReadCardSerial()) 
        return; /* if the reading fails walk program again */
      for (auto i = 0; i < UID_WIDTH; i++)
        card_uid[i] = reader.uid.uidByte[i];
      
      /* Print Read UID */
// DEBUG      
      for (auto uid_byte : card_uid)
        Serial.print(uid_byte, HEX);
      Serial.println("");
// END OF DEBUG      
      /* END of READING */
      reader.PICC_HaltA();

// TODO start the PC with "PWR_BTN" pin
  }
      
}
