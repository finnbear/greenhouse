#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <SparkFun_Si7021_Breakout_Library.h>

Adafruit_NeoPixel led = Adafruit_NeoPixel(1, 4, NEO_GRB + NEO_KHZ800);

void setLed(byte red, byte green, byte blue) {
  uint32_t color = led.Color(red, green, blue);
  led.setPixelColor(0, color);
  led.show();
}

ADC_MODE(ADC_VCC);

struct Settings {
  char ssid[32];
  char pass[32];
  char channel[7];
  char key[17];
};

Settings settings;

WiFiClient client;

Weather onboardSi7021;

void setup() {
  Serial.begin(9600);
  EEPROM.begin(128);

  EEPROM.get(0, settings);

  ThingSpeak.begin(client);

  setLed(0, 127, 0);
  onboardSi7021.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    setLed(0, 0, 127);

    // Read onboard Si7021
    ThingSpeak.setField(2, onboardSi7021.getRH());
    ThingSpeak.setField(1, onboardSi7021.getTemp() + 273.15f);

    ThingSpeak.setField(7, WiFi.RSSI());
    ThingSpeak.setField(8, ESP.getVcc() / 1000.0f);
    

    ThingSpeak.writeFields(atol(settings.channel), settings.key);

    setLed(0, 0, 0);

    delay(15000);
  } else {
    Serial.println("Connecting...");
    Serial.print("SSID: ");
    Serial.println(settings.ssid);
  
    WiFi.begin(settings.ssid, settings.pass);
  
    Serial.print("Waiting...");
    setLed(127, 0, 0);

    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }

    setLed(0, 0, 0);
    
    Serial.println("");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }
}
