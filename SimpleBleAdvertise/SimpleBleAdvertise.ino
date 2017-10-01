#include "ESP32BleAdvertise.h"
SimpleBLE ble;

void setup() {
  Serial.begin(115200);
  ble.begin("LedMatrix");  //sets the device name
}

bool newData = false;

String str = "";

void loop() {
  if (Serial.available()) {
    str = "";
  }
  while (Serial.available()) {
    str += (char)Serial.read();
    newData = true;
  }
  if (newData) {
    Serial.println(str);
    ble.end();
    str.trim();
    ble.begin(str);
    ble.advertise(str);
    newData = false;
    
  }
}
