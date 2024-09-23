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
    digitalWrite(relay, LOW);
  }
  // Initial Test
  delay(1000); 
  for (int relay:relays1)
  {
    digitalWrite(relay, HIGH);
    delay(1000);
    digitalWrite(relay, HIGH);
  }
  delay(1000);
}


void loop() {

  for(int arrayPos = 0; arrayPos<sizeof(data); arrayPos++)
  {
    
    unsigned char sv0 = pgm_read_byte_near(data + arrayPos);
    potVal = analogRead(A0);
    ddt = map(potVal, 0, 1023,700,50); //variable, min in, max in, max out, min out
    Serial.println(ddt);

    Serial.print(sv0);
    Serial.print(" ");

    // sv0 - Control Relay 1-8
    if(sv0 >= 0 && sv0 <10){
        for (int i:relays1){
          digitalWrite(i, HIGH);
        }
      digitalWrite(relays1[sv0],LOW); 
    }

    // if(sv0 == 8){
    //   for (int i:relays1){
    //     digitalWrite(i, LOW);
    //   }
    // }
    delay(ddt*0.1 * sv0);
    for (int i:relays1){
        digitalWrite(i, HIGH);
      }
    delay(ddt*0.2 * sv0);
  }
  Serial.println(".... no more data");
}