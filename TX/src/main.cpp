#include "arduino.h"
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

#define LEFT_PIN 2
#define RIGHT_PIN 3
#define FORWARD_PIN 4
#define BACKWARD_PIN 5

#define RF24_CS 10
#define RF24_CE 9

struct data{
    bool forward;
    bool backward;
    bool left;
    bool right;
};

data button_data;

RF24 radio(RF24_CE, RF24_CS);

RF24Network network(radio);

const uint16_t this_node = 01;
const uint16_t other_node = 00;
const unsigned long interval = 100;

unsigned long last_sent;
unsigned long packets_sent;


void setup(void) {
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("RF24Network/examples/helloworld_tx/"));

  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (1);
  }
  radio.setChannel(90);
  network.begin(/*node address*/ this_node);
}

void loop() {

  network.update();

  unsigned long now = millis();

  button_data.forward = digitalRead(2) == HIGH;
  button_data.backward = digitalRead(3) == HIGH;
  button_data.left = digitalRead(4) == HIGH;
  button_data.right = digitalRead(5) == HIGH;

  if (now - last_sent >= interval) {
    last_sent = now;
    Serial.print(F("Sending... "));
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header, &button_data, sizeof(button_data));
    Serial.println(ok ? F("ok.") : F("failed."));
  }
}
