/* Necessary Libs */
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <deprecated.h>
/* END of Dependencies */

/* Enumerate of OUTPUT pins name */
enum { MFRC522_SPI_SS, MFRC522_RST, PWR_BTN };
/* Array of OUTPUT pins */
const uint8_t output_pins[] = { 
  3, /* MFRC522_SPI_SS - SlaveSelect SPI pin for MFRC522 */
  4, /* MFRC522_RST    - Pin for resetting MFRC522 */
  5  /* PWR_BTN        - Pin that is connected as substitute of the signal from case's power button */
  };

/* Init (Executed once) Code */
void setup(void) {
  /* set all OUTPUT pins as OUTPUT */
  for (auto pin : output_pins) 
    pinMode(pin, OUTPUT);
}

/* Endlessly running code */
void loop(void) {
  digitalWrite(output_pins[PWR_BTN], HIGH);
}
