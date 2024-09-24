// data file
#include <avr/pgmspace.h>
#include "data.h"
#include "data1000.h"


#define bpm_left 300
#define left_on_percent 0.6

#define left_cycle_time_ms 60000/bpm_left
#define left_on_time_ms left_on_percent*left_cycle_time_ms
#define left_off_time_ms left_cycle_time_ms - left_on_time_ms

#define bpm_right 200
#define right_on_percent 0.3

#define right_cycle_time_ms 60000/bpm_right
#define right_on_time_ms right_on_percent*right_cycle_time_ms
#define right_off_time_ms right_cycle_time_ms - right_on_time_ms

//define relays 
int relays1[] = {44,45,46,47,48,49,50,51,52,53};
//                    S  S  L  L
int right_relays[] = {47,53,46,46}; //52
int left_relays[] =  {48,51,49,50};
int right_light = 44;
int left_light = 45;

unsigned char left_relaypattern[] = {0b0000,0b0010,0b0011,0b0100,0b0101,0b0110,0b0101,0b1011,0b1111};
unsigned char right_relaypattern[] = {0b0000,0b0010,0b0011,0b0100,0b0101,0b0110,0b0101,0b1100,0b1111};
//unsigned char right_relaypattern[] = {0b0000,0b0000,0b0001,0b0001,0b0010,0b0010,0b0100,0b1000,0b1000};
//unsigned char left_relaypattern[] = {0b0000,0b0000,0b0001,0b0001,0b0010,0b0010,0b0100,0b1000,0b1000};


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

int left_array_pos = 0;
unsigned char leftval = 0;
unsigned long left_pulse_time = 0;
bool left_on = false; // 0 is off, 1 is on
int left_delay = left_off_time_ms;

int right_array_pos = 0;
unsigned char rightval = 0;
unsigned long right_pulse_time = 0;
bool right_on = false; // 0 is off, 1 is on
int right_delay = right_off_time_ms;

void loop() {
  //Serial.println(millis());
  if ((millis() - left_pulse_time) > left_delay){ //if the delay period is over
    if (left_on){ //if we're on, turn off all the relays
      // Serial.println("off");
      for (int i:left_relays){
        digitalWrite(i, HIGH);
      }
      left_pulse_time = millis();
      left_delay = left_off_time_ms;
      left_on = false;
    } else { //otherwise, turn on the specified relays based on the data pattern
      // Serial.println("on");
      leftval = pgm_read_byte_near(data_200 + left_array_pos);
      Serial.print("L");
      Serial.print(leftval);
      Serial.print(" ");
      if(leftval >= 0 && leftval <=8){
        unsigned char left_data_pattern = left_relaypattern[leftval];
        int i = 0;
        for (int relay: left_relays){
          digitalWrite(relay, !((left_data_pattern >> i) & 0b0001));

          Serial.print((left_data_pattern >> i) & 0b0001);
          i++;
        }
      }
      
      if (++left_array_pos >= sizeof(data_200) ){left_array_pos = 0; Serial.println(".... no more data on left");} //increment array pos, if there is no data left, set to 0 
      left_delay = left_on_time_ms;
      Serial.println("");
      left_pulse_time = millis();
      left_on = true;
    }
  }

  if ((millis() - right_pulse_time) > right_delay){ //if the delay period is over
    if (right_on){ //if we're on, turn off all the relays
      // Serial.println("off");
      for (int i:right_relays){
        digitalWrite(i, HIGH);
      }
      right_pulse_time = millis();
      right_delay = right_off_time_ms;
      right_on = false;
    } else { //otherwise, turn on the specified relays based on the data pattern
      // Serial.println("on");
      rightval = pgm_read_byte_near(data_1000 + right_array_pos);
      Serial.print("R");
      Serial.print(rightval);
      Serial.print(" ");
      if(rightval >= 0 && rightval <=8){
        unsigned char right_data_pattern = right_relaypattern[rightval];
        int i = 0;
        for (int relay: right_relays){
          digitalWrite(relay, !((right_data_pattern >> i) & 0b0001));

          Serial.print((right_data_pattern >> i) & 0b0001);
          i++;
        }
      }
      
      if (++right_array_pos >= sizeof(data_1000) ){right_array_pos = 0; Serial.println(".... no more data on right");} //increment array pos, if there is no data right, set to 0 
      right_delay = right_on_time_ms;
      Serial.println("");
      right_pulse_time = millis();
      right_on = true;
    }
  }

  update_light(leftval >= rightval);
  
}

bool current_is_left = true;
unsigned long last_light_time = 0;
void update_light(bool is_left){
 if ((millis() - last_light_time) > 1000){ //do not update lights faster than 1hz for epilepsy reasons
  if (is_left != current_is_left){
    last_light_time = millis();
    current_is_left = is_left;
    if (is_left){
      digitalWrite(left_light, LOW);
      digitalWrite(right_light, HIGH);
    } else {
      digitalWrite(left_light, HIGH);
      digitalWrite(right_light, LOW);
    }
    
  }
 }
}
