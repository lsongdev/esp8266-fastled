#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 33
#define BTN_PIN 22
#define NUM_LEDS 30
#define MODES 3

CRGB leds[NUM_LEDS];
int currentMode = 0;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(50);
  pinMode(BTN_PIN, INPUT_PULLUP);
}

void mode0() { // 单色流水灯
  static int pos = 0;
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  leds[pos] = CRGB::Red;
  FastLED.show();
  pos = (pos + 1) % NUM_LEDS;
  delay(100);
}

void mode1() { // 彩虹流水灯
  static uint8_t hue = 0;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue + (i * 255/NUM_LEDS), 255, 255);
  }
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
  int reading = digitalRead(BTN_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) {
      currentMode = (currentMode + 1) % MODES;
    }
  }
  
  lastButtonState = reading;
  
  switch(currentMode) {
    case 0: mode0(); break;
    case 1: mode1(); break;
    case 2: mode2(); break;
  }
}