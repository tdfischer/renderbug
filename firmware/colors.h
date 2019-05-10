#pragma once
#include "FastLED/FastLED.h"

typedef struct ColorInfo {
  const char *name;
	NSFastLED::CRGB rgb;
} ColorInfo;

ColorInfo colorForName(const char *name);
