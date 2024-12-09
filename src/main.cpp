#include <Arduino.h>
#include <FastLED.h>
#include <KButton.h>

// #define LED_PIN 2
#define LED_PIN 33
#define BTN_PIN 22
#define NUM_LEDS 300
#define MODES 3

CRGB leds[NUM_LEDS];
int currentMode = 0;
KButton button(BTN_PIN);

void handleClick(){
  currentMode = (currentMode + 1) % MODES;
}

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(50);
  button.attachClick(handleClick);
}

void mode0() { // 单色流水灯
  static int pos = 0;
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  leds[pos] = CRGB::Red;
  FastLED.setBrightness(50);
  FastLED.show();
  pos = (pos + 1) % NUM_LEDS;
  delay(100);
}

void mode1() { // 彩虹流水灯
  static uint8_t hue = 0;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue + (i * 255/NUM_LEDS), 255, 255);
  }
  FastLED.setBrightness(50);
  FastLED.show();
  hue++;
  delay(20);
}

void mode2() { // 呼吸灯
  static uint8_t brightness = 0;
  static int8_t delta = 1;
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.setBrightness(brightness);
  FastLED.show();
  
  brightness += delta;
  if(brightness == 0 || brightness == 50) delta = -delta;
  delay(10);
}

void loop() {
  button.tick();
  switch(currentMode) {
    case 0: mode0(); break;
    case 1: mode1(); break;
    case 2: mode2(); break;
  }
}