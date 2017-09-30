#include "MKRLora.h"

LoRaModem modem;

// Uncomment if using the Murata chip as a module
// LoRaModem modem(Serial1);

// Replace with keys obtained from TheThingsNetwork console
String appEui = "70B3D57ED0007670";
String appKey = "61149F0FD6B727697FD2755186AF6810";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  if (!modem.begin()) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  modem.restart();
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  /*
    int connected = modem.joinOTAA(appEui, appKey);
    if (!connected) {
      Serial.println("Something went wrong; are you indoor? Move near a window and retry");
      while (1) {}
    }
  */

  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  // NOTE: independently by this setting the modem will
  // not allow to send more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.
}

void loop() {
  Serial.println();
  Serial.println("Enter a message to send to network");
  Serial.println("(make sure that end-of-line 'NL' is enabled)");

  while (!Serial.available());
  String msg = Serial.readStringUntil('\n');

  Serial.println();
  Serial.print("Sending: " + msg + " - ");
  for (unsigned int i = 0; i < msg.length(); i++) {
    Serial.print(msg[i] >> 4, HEX);
    Serial.print(msg[i] & 0xF, HEX);
    Serial.print(" ");
  }
  Serial.println();

  int err;
  modem.beginPacket();
  modem.print(msg);
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
    Serial.println("(remember: you can send a limited amount of messages");
    Serial.println("per hour, usually no more than 1 msg every 2 minutes)");
  }
  delay(1000);
  if (!modem.available()) {
    Serial.println("No downlink message received at this time.");
    return;
  }
  String rcv;
  rcv.reserve(64);
  while (modem.available()) {
    rcv += (char)modem.read();
  }
  Serial.print("Received: " + rcv + " - ");
  for (unsigned int i = 0; i < rcv.length(); i++) {
    Serial.print(rcv[i] >> 4, HEX);
    Serial.print(rcv[i] & 0xF, HEX);
    Serial.print(" ");
  }
  Serial.println();
}
