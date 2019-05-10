#pragma once

class Blob {
  uint16_t m_pos;
  int8_t m_velocity;
  uint8_t m_hue;
  int16_t m_brightness;
  uint8_t m_saturation;
  int8_t m_fadeDir;
public:
  Blob()
    : m_pos(0),
      m_velocity(1),
      m_hue(0),
      m_brightness(1),
      m_saturation(200),
      m_fadeDir(1) {}

  void setSaturation(uint8_t v) {
    m_saturation = v;
  }
      
  void setPos(uint16_t p) {
    m_pos = p;
    m_brightness = p % 120 + 1;
  }

  void setHue(uint8_t p) {
    m_hue = p;
  }

  void setBrightness(uint8_t p) {
    m_brightness = p;
  }

  void setVelocity(int8_t v) {
    m_velocity = v;
  }

  void update() {
      m_pos += m_velocity;
      m_hue += 1;
      m_brightness += m_fadeDir;
      if (m_brightness >= 255 || m_brightness <= 0) {
        m_fadeDir *= -1;
      }
  }
  
  void render(Display* display) const {
      const uint8_t width = 25;
      auto map = display->coordinateMapping();
      // Grab the physical pixel we'll start with
      PhysicalCoordinates startPos = map->virtualToPhysicalCoords({m_pos, 0});
      PhysicalCoordinates endPos = map->virtualToPhysicalCoords({m_pos + width, 0});
      int scaledWidth = std::abs(endPos.x - startPos.x);

      for(uint8_t i = 0;i < scaledWidth; i++) {
        // Blobs desaturate towards their tail
        NSFastLED::CHSV blobColor(m_hue, m_saturation, NSFastLED::quadwave8((i / (double)scaledWidth) * m_brightness));

        PhysicalCoordinates pos{startPos.x + (i*m_fadeDir), 0};

        NSFastLED::CRGB src(display->pixelAt(pos));
        display->pixelAt(pos) = NSFastLED::blend(NSFastLED::CRGB(blobColor), src, 200);
      }
  }
};
