
#include <SigFox.h>
#include <ArduinoLowPower.h>

static const uint8_t lsm_init[] = {
  B01011111, 
  B00000000
};

void setup() {
  Serial.begin(9600);
  while (!Serial) {};

  // Uncomment this line and comment begin() if you are working with a custom board
  //if (!SigFox.begin(SPI1, 30, 31, 33, 28, LED_BUILTIN)) {
  if (!SigFox.begin()) {
    Serial.println("Shield error or not present!");
    return;
  }
  // Enable debug led and disable automatic deep sleep
  // Comment this line when shipping your project :)
  SigFox.debug();

  String version = SigFox.SigVersion();
  String ID = SigFox.ID();
  String PAC = SigFox.PAC();

  // Display module informations
  Serial.println("MKRFox1200 Sigfox first configuration");
  Serial.println("SigFox FW version " + version);
  Serial.println("ID  = " + ID);
  Serial.println("PAC = " + PAC);

  Serial.println("");

  Serial.print("Module Setup: ");
  Serial.println(SigFox.internalTemperature());

  Serial.println("Register your board on https://backend.sigfox.com/activate with provided ID and PAC");

  delay(100);

  // Send the module to the deepest sleep
  SigFox.end();

  Serial.println("Message is going to be sent");

  Serial.println("Getting the response will take up to 50 seconds");
  Serial.println("The LED will blink while the operation is ongoing");

  // Example of send and read response
  //String(lsm_init[0])
  SigFox.begin();
  delay(100);

  SigFox.beginPacket();
  SigFox.write((byte)B01010111);
  int ret = SigFox.endPacket();
}

void loop()
{
}

