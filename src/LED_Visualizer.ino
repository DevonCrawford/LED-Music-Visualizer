#include "FastLED.h"

// Define constants
#define NUM_LEDS 300
#define UPDATE_LEDS 8
#define COLOR_SHIFT 180000
#define MIN_BRIGHTNESS 0.2
#define MAX_BRIGHTNESS 1.0
#define BRIGHTNESS_DIVIDER 614.0

// Define LED array and color structure
CRGB leds[NUM_LEDS] = {};

struct Color {
  uint8_t r, g, b;
};

// Define variables
unsigned long colorShiftInterval = COLOR_SHIFT;
int shiftC = 0;
int mulC = 2;

// Setup function
void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  // Add NEOPIXEL LEDs to FastLED library
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // Set input pins for pitch and brightness
  pinMode(PITCH_PIN, INPUT);
  pinMode(BRIGHT_PIN, INPUT);

  // Fill LED array with black color
  fillLedsWithBlack();
  // Display initial state
  FastLED.show();
}

// Main loop function
void loop() {
  // Shift color spectrum at regular intervals
  if (millis() >= colorShiftInterval) {
    colorShiftInterval += COLOR_SHIFT;
    shiftColorSpectrum();
  }

  // Update LEDs and set color based on pitch and brightness
  updateLeds();
  setUpdatedLedsColor(pitchConv((analogRead(PITCH_PIN) * 2) + shiftC, analogRead(BRIGHT_PIN)));
  // Display the updated LEDs
  FastLED.show();
}

// Convert brightness to a normalized value
double convBrightness(int b) {
  double c = b / BRIGHTNESS_DIVIDER;
  // Ensure brightness is within valid range
  return (c < MIN_BRIGHTNESS) ? 0 : (c > MAX_BRIGHTNESS) ? 1.0 : c;
}

// Convert pitch and brightness to RGB color
Color pitchConv(int p, int b) {
  Color c;
  // Normalize brightness
  double bright = convBrightness(b);
  // Map pitch values to RGB components
  int mappedRed = mapLinear(p, 239, 250, 0, 255);
  int mappedGreen = mapLinear(p, 398, 653, 255, 0);
  int mappedBlue = mapLinear(p, 398, 653, 0, 255);

  // Set color based on pitch range
  if (p < 40) setColor(&c, 255, 0, 0);
  else if (p <= 77) setColor(&c, 255, 0, mapLinear(p, 40, 77, 0, 255));
  else if (p <= 205) setColor(&c, mappedRed, 0, 255);
  else if (p <= 238) setColor(&c, 0, mapLinear(p, 206, 238, 0, 255), 255);
  else if (p <= 250) setColor(&c, mappedRed, 255, 255);
  else if (p <= 270) setColor(&c, 255, 255, 255);
  else if (p <= 398) setColor(&c, mapLinear(p, 271, 398, 255, 0), 255, mappedBlue);
  else setColor(&c, 255, 0, 0);

  // Apply brightness to the color
  setColor(&c, c.r * bright, c.g * bright, c.b * bright);
  return c;
}

// Set RGB values for a Color structure
inline void setColor(Color *c, uint8_t r, uint8_t g, uint8_t b) {
  c->r = r;
  c->g = g;
  c->b = b;
}

// Shift the color spectrum
void shiftColorSpectrum() {
  shiftC += 200;
  // Reset multiplier when it reaches a certain value
  mulC = (shiftC >= 600) ? 2 : mulC + 1;
}

// Fill the LED array with black color
void fillLedsWithBlack() {
  std::fill(leds, leds + NUM_LEDS, CRGB(0, 0, 0));
}

// Rotate LED array to simulate motion
void updateLeds() {
  std::rotate(leds, leds + (NUM_LEDS - UPDATE_LEDS), leds + NUM_LEDS);
}

// Set color for the updated LEDs
void setUpdatedLedsColor(Color nc) {
  std::fill(leds, leds + UPDATE_LEDS, CRGB(nc.r, nc.g, nc.b));
}

// Linear mapping function
inline int mapLinear(int value, int inMin, int inMax, int outMin, int outMax) {
  return static_cast<int>((value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin);
}
