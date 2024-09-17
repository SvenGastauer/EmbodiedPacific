// FOR SD CARD
#include <SPI.h>
#include <SD.h>
const int chipSelect = 53;
//Define Sv values
int sv0;
int sv1;

#define pot A0
int potVal;
int ddt; //dynamic delay time

//define relays 
int relays1[] = {2,3,4,5,6,7,8,9};
int relays2[] = {10,11,12,13,30,31,32,33};

void setup() {
  Serial.begin(115200);
  //SD CARD TESTS
  Serial.println(".... parse floats out of sd file ....");
  Serial.print("Compiler: ");
  Serial.print(__VERSION__);
  Serial.print(", Arduino IDE: ");
  Serial.println(ARDUINO);
  Serial.print("Created: ");
  Serial.print(__TIME__);
  Serial.print(", ");
  Serial.println(__DATE__);
  Serial.println(__FILE__);

  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  // RELAY TEST
   for (int i=0; i<8; i++)
  {
    pinMode(relays1[i], OUTPUT);
  }
  for (int i=0; i<8; i++)
  {
    pinMode(relays2[i], OUTPUT);
  }
  // Initial Test
  delay(500); 
  for (int i=0; i<8; i++)
  {
    digitalWrite(relays1[i], LOW);
    digitalWrite(relays2[i], LOW);
    delay(200);
  }
  for (int i=0; i<8; i++)
  {
    digitalWrite(relays1[i], HIGH);
    digitalWrite(relays2[i], HIGH);
    delay(200);
  }
  delay(500); 
}


void loop() {
  File fn0 = SD.open("sv200.txt"); 
  File fn1 = SD.open("sv1000.txt");
  if (fn0 && fn1)
  {
    while (fn0.available() && fn1.available())
    {
      potVal = analogRead(A0);
      ddt = map(potVal, 0, 1023,700,50); //variable, min in, max in, max out, min out
      Serial.println(ddt);
  
  
      //parse integers from file:
      sv0 = fn0.parseInt();
      sv1 = fn1.parseInt();
      Serial.print(sv0);
      Serial.print(" ");
      //sv1=analogRead(pot1);
      Serial.println(sv1);

      // sv0 - Control Relay 1-8
      if(sv0 >= 0 && sv0 <8){
          for (int i=0; i<8; i++){
            digitalWrite(relays1[i], HIGH);
          }
        digitalWrite(relays1[sv0],LOW); 
      }

      if(sv0 == 8){
        for (int i=0; i<8; i++){
          digitalWrite(relays1[i], LOW);
        }
      }
      // sv1 - Control Relay 8-16
      if(sv1 >= 0 && sv1 <8){
          for (int i=0; i<8; i++){
            digitalWrite(relays2[i], HIGH);
          }
        digitalWrite(relays2[sv1],LOW); 
      }
      if(sv1 == 8){
        for (int i=0; i<8; i++){
          digitalWrite(relays2[i], LOW);
        }
      }
  delay(ddt);
    }
    Serial.println(".... no more data");
    fn0.close();
    fn1.close();
  }
}