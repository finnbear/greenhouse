#include <EEPROM.h>

struct Settings {
  char ssid[32];
  char pass[32];
};

void setup() {
  Serial.begin(9600);

  Serial.println("Beginning EEPROM...");
  EEPROM.begin(128);

  Settings settings = {"SSID", "PASS"};

  Serial.println("Writting settings to EEPROM...");
  EEPROM.put(0, settings);

  //Serial.println("Commiting EEPROM...");
  //EEPROM.commit();

  Serial.println("Checking EEPROM...");
  for (int i = 0; i < 128; i++) {
    Serial.write(EEPROM.read(i));
  }
}

void loop() {
  
}
