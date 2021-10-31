#include <Adafruit_NeoPixel.h>

// NeoPixel
#define LED_PIN   4
#define LED_COUNT 2
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void neo_init(){
  strip.begin();
  strip.setBrightness(1);
  strip.setPixelColor(0, strip.Color(0, 100, 255));
  strip.show();
}

void neo_clear(){
  strip.clear();
  strip.show();
}