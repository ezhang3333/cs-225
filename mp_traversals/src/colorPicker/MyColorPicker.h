#pragma once

#include "ColorPicker.h"
#include "cs225/HSLAPixel.h"
#include "../Point.h"

using namespace cs225;

/**
 * A color picker class using your own color picking algorithm
 */
class MyColorPicker : public ColorPicker {
public:
  MyColorPicker(double hueStart, double hueStep, double saturation, double lightness);
  HSLAPixel getColor(unsigned x, unsigned y);

private:
  double hueStart_;
  double hueStep_;
  double saturation_;
  double lightness_;
};
