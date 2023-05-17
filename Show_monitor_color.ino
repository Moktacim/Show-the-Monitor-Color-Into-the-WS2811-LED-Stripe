#include <FastLED.h>

#define LED_PIN 6
#define LED_PIN1 9
#define NUM_LEDS 33

CRGB leds[NUM_LEDS];

void setup() 
{
  Serial.begin(115200);
  FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2811, LED_PIN1, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
}

void loop() 
{
  static byte buffer[1024];
  static int index = 0;
  
  // Read the data from the serial port
  while (Serial.available() > 0) {
    byte data = Serial.read();
    // data.trim();
    
    if (data == '[') {
      index = 0; // reset the buffer index
    }
    else if (data == ']') {
      // Parse the buffer
      int i = 0;
      char *p = strtok((char*)buffer, " ,");
      while (p != NULL && i < NUM_LEDS * 3) {
        int value = atoi(p);
        int index = i / 3;
        int channel = i % 3;
        if (index >= 0 && index < NUM_LEDS && channel >= 0 && channel < 3) {
          if (channel == 0) leds[index].r = value;
          if (channel == 1) leds[index].g = value;
          if (channel == 2) leds[index].b = value;
        }
        i++;
        p = strtok(NULL, " ,");
      }     
      FastLED.show();
    }
    else {
      buffer[index++] = data;
    }
  }
  FastLED.clear();
}
