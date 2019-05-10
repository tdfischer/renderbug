#include "FastLED/FastLED.h"
#include "./Geometry.h"

class Display;

class Surface {
public:
    Surface(Display* dpy, const VirtualCoordinates& start, const VirtualCoordinates& end);

    Surface& operator=(const NSFastLED::CRGB& color);
    Surface& operator+=(const NSFastLED::CRGB& color);

    void paintWith(std::function<void(NSFastLED::CRGB&)> func);

    const PhysicalCoordinates start;
    const PhysicalCoordinates end;

private:
    Display* m_display;
};
