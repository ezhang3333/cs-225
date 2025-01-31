#include "cs225/HSLAPixel.h"
#include "../Point.h"

#include "ColorPicker.h"
#include "MyColorPicker.h"
#include <cmath>

using namespace cs225;

/**
 * Picks the color for pixel (x, y).
 * Using your own algorithm
 */

MyColorPicker::MyColorPicker(double hueStart, double hueStep, double saturation, double lightness) 
    : hueStart_(hueStart), hueStep_(hueStep), saturation_(saturation), lightness_(lightness) {}

HSLAPixel MyColorPicker::getColor(unsigned x, unsigned y)
{
  double hue = hueStart_ + (x+y) * hueStep_;
  hue = fmod(hue, 360.0);
  return HSLAPixel(hue, saturation_, lightness_);  
}