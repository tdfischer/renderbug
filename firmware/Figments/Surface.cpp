#include "./Surface.h"
#include "./Display.h"

Surface::Surface(Display* dpy, const VirtualCoordinates& start, const VirtualCoordinates& end)
  : start(dpy->coordinateMapping()->virtualToPhysicalCoords(start)), 
    end(dpy->coordinateMapping()->virtualToPhysicalCoords(end)),
    m_display(dpy)
{
}

Surface&
Surface::operator=(const NSFastLED::CRGB& color)
{
    paintWith([&](NSFastLED::CRGB& pixel) {
        pixel = color;
    });
    return *this;
}

Surface&
Surface::operator+=(const NSFastLED::CRGB& color)
{
    paintWith([&](NSFastLED::CRGB& pixel) {
        pixel += color;
    });
    return *this;
}

void
Surface::paintWith(std::function<void(NSFastLED::CRGB&)> func)
{
    for(uint8_t x = start.x; x <= end.x; x++) {
        for(uint8_t y = start.y; y <= end.y; y++) {
            func(m_display->pixelAt(PhysicalCoordinates{x, y}));
        }
    }
}
