// data file
#include <avr/pgmspace.h>
#include "data0.h"

//Define Sv values
int sv0;
int sv1;

#define pot A0
int potVal;
int ddt = 1000; //dynamic delay time

//define relays 
int relays1[] = {44,45,46,47,48,49,50,51,52,53};

void setup() {
  Serial.begin(115200);
  //SD CARD TESTS
  Serial.println(".... parse floats out of header file ....");
  Serial.print("Compiler: ");
  Serial.print(__VERSION__);
  Serial.print(", Arduino IDE: ");
  Serial.println(ARDUINO);
  Serial.print("Created: ");
  Serial.print(__TIME__);
  Serial.print(", ");
  Serial.println(__DATE__);
  Serial.println(__FILE__);



  // RELAY TEST
   for (int relay:relays1)
  {
    pinMode(relay, OUTPUT);
    digitalWrite(relay, HIGH);
  }
  // Initial Test
  delay(500); 
  for (int relay:relays1)
  {
    digitalWrite(relay, LOW);
    delay(1000);
    digitalWrite(relay, HIGH);
  }
  delay(100);
}