#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

#define TURN_MOD1 4
#define TURN_VAL 5
#define TURN_MOD2 3

#define DRIVE_VAL 2
#define DRIVE_MOD2 6
#define DRIVE_MOD1 7

#define RF24_CS 10
#define RF24_CE 9

RF24 radio(RF24_CE, RF24_CS);

RF24Network network(radio);
const uint16_t this_node = 00;
const uint16_t other_node = 01;

struct data{
    bool forward;
    bool backward;
    bool left;
    bool right;
};

data button_data;

void setup(void) {
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("RF24Network/examples/helloworld_rx/"));

  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (1);
  }
  radio.setChannel(90);
  network.begin(/*node address*/ this_node);
}

void update_car(int state) {
    if(state == 1){
        //Serial.println("Car is running");
        if(button_data.forward){
            digitalWrite(DRIVE_MOD1, HIGH);
            digitalWrite(DRIVE_MOD2, LOW);
            digitalWrite(DRIVE_VAL, HIGH);
        }else if (button_data.backward){
            digitalWrite(DRIVE_MOD1, LOW);
            digitalWrite(DRIVE_MOD2, HIGH);
            digitalWrite(DRIVE_VAL, HIGH);
        }else{
            digitalWrite(DRIVE_MOD1, LOW);
            digitalWrite(DRIVE_MOD2, LOW);
            digitalWrite(DRIVE_VAL, LOW);
        }

        if(button_data.left){
            digitalWrite(TURN_MOD1, HIGH);
            digitalWrite(TURN_MOD2, LOW);
            digitalWrite(TURN_VAL, HIGH);
        }else if (button_data.right){
            digitalWrite(TURN_MOD1, LOW);
            digitalWrite(TURN_MOD2, HIGH);
            digitalWrite(TURN_VAL, HIGH);
        }else{
            digitalWrite(TURN_MOD1, LOW);
            digitalWrite(TURN_MOD2, LOW);
            digitalWrite(TURN_VAL, LOW);
        }

    } else {
        //Serial.println("Car is stopped");
        digitalWrite(TURN_MOD1, LOW);
        digitalWrite(TURN_MOD2, LOW);
        digitalWrite(TURN_VAL, LOW);
        digitalWrite(DRIVE_MOD1, LOW);
        digitalWrite(DRIVE_MOD2, LOW);
        digitalWrite(DRIVE_VAL, LOW);
    }
}

void loop(void) {

  network.update();

  while (network.available()) {
    RF24NetworkHeader header;
    network.read(header, &button_data, sizeof(button_data));
    update_car(1);
  }

  update_car(0);

}
