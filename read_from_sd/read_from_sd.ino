// data file
#include <avr/pgmspace.h>
#include "data.h"

//Define Sv values
int sv0;
int sv1;


int ddt = 200; //dynamic delay time

#define bpm 300
#define on_percent 0.6

#define cycle_time_ms 60000/bpm
#define on_time_ms on_percent*cycle_time_ms
#define off_time_ms cycle_time_ms - on_time_ms

//define relays 
int relays1[] = {44,45,46,47,48,49,50,51,52,53};
//                    S  S  L  L
int right_relays[] = {47,53,46,52};
int left_relays[] =  {48,51,49,50};
int right_light = 44;
int left_light = 45;

unsigned char left_relaypattern[] = {0b0000,0b0010,0b0011,0b0100,0b0101,0b0110,0b0111,0b1111,0b1111};
unsigned char right_relaypattern[] = {0b0000,0b0010,0b0011,0b0100,0b0101,0b0110,0b0111,0b1100,0b1111};

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
  for (int relay:relays1)
  {
    digitalWrite(relay, LOW);
    delay(100);
    digitalWrite(relay, HIGH);
  }

}


void loop() {

  for(int arrayPos = 0; arrayPos<sizeof(data); arrayPos++)
  {
    
    unsigned char sv0 = pgm_read_byte_near(data + arrayPos);

    Serial.print(sv0);
    Serial.print(" ");

    // sv0 - Control Relay 1-8
    if(sv0 >= 0 && sv0 <=8){
      unsigned char left_data_pattern = left_relaypattern[sv0];
      int i = 0;
        for (int relay: left_relays){
          digitalWrite(relay, !((left_data_pattern >> i) & 0b0001));

          Serial.print((left_data_pattern >> i) & 0b0001);
          Serial.print(" ");
          i++;
        }
    }

    delay(on_time_ms);
    for (int i:left_relays){
        digitalWrite(i, HIGH);
      }
    delay(off_time_ms);
    Serial.println("");
  }
  Serial.println(".... no more data");
}