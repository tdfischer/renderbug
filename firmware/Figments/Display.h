#pragma once
#include "FastLED/FastLED.h"

#include "Geometry.h"

struct CoordinateMapping {
    virtual VirtualCoordinates physicalToVirtualCoords(const PhysicalCoordinates localCoords) const = 0;
    virtual PhysicalCoordinates virtualToPhysicalCoords(const VirtualCoordinates virtualCoords) const = 0;
    virtual int physicalCoordsToIndex(const PhysicalCoordinates localCoords) const = 0;
    virtual unsigned int physicalPixelCount() const = 0;
};

struct LinearCoordinateMapping: CoordinateMapping {
    unsigned int pixelCount = 1;
    unsigned int startPixel = 0;
    LinearCoordinateMapping() {}
    LinearCoordinateMapping(unsigned int count, unsigned int start) : pixelCount(count), startPixel(start) {}
    VirtualCoordinates physicalToVirtualCoords(const PhysicalCoordinates localCoords) const {
        return VirtualCoordinates{(uint8_t)((localCoords.x) / pixelCount), 0};
    }

    PhysicalCoordinates virtualToPhysicalCoords(const VirtualCoordinates virtualCoords) const {
        return PhysicalCoordinates{NSFastLED::scale8(pixelCount, virtualCoords.x), 0};
    }

    int physicalCoordsToIndex(const PhysicalCoordinates localCoords) const override {
        return localCoords.x + startPixel;
    }

    unsigned int physicalPixelCount() const {
        return pixelCount;
    }
};

class Display {
  public:

    Display(NSFastLED::CRGB* pixels, int pixelCount, const CoordinateMapping* map)
      : m_pixels(pixels), m_pixelCount(pixelCount), m_coordMap(map) {}

    NSFastLED::CRGB& pixelAt(const PhysicalCoordinates coords);
    NSFastLED::CRGB& pixelAt(const VirtualCoordinates coords);
    NSFastLED::CRGB& pixelAt(int idx);

    int pixelCount() const;
    NSFastLED::CRGB* pixelBacking() const;
    const CoordinateMapping* coordinateMapping() const;
    void clear();
    void clear(const NSFastLED::CRGB& color);
    void clear(VirtualCoordinates& start, VirtualCoordinates& end, const NSFastLED::CRGB& color);

  private:
    NSFastLED::CRGB* m_pixels;
    int m_pixelCount;
    const CoordinateMapping* m_coordMap;
};
