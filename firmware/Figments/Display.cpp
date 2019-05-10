#include "Display.h"
#include <math.h>

using namespace NSFastLED;

int
Display::pixelCount() const
{
    return m_pixelCount;
}

CRGB*
Display::pixelBacking() const
{
    return m_pixels;
}

void
Display::clear()
{
    clear(CRGB(0, 0, 0));
}

void
Display::clear(const CRGB& color)
{
    for(int i = 0; i < m_pixelCount;i++) {
        m_pixels[i] = color;
    }
}

CRGB&
Display::pixelAt(const PhysicalCoordinates coords)
{
    return pixelAt(m_coordMap->physicalCoordsToIndex(coords));
}

CRGB&
Display::pixelAt(const VirtualCoordinates coords)
{
    return pixelAt(m_coordMap->virtualToPhysicalCoords(coords));
}

CRGB&
Display::pixelAt(int idx)
{
    const int kx = idx % pixelCount();
    if (kx < 0) {
        return m_pixels[pixelCount() + 1 + kx];
    } else {
        return m_pixels[kx];
    }
}

const CoordinateMapping*
Display::coordinateMapping() const
{
    return m_coordMap;
}
