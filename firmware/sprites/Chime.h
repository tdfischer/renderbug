#pragma once

using namespace NSFastLED;

template<int ChimeLength>
class Chime {
  uint16_t m_pos;
  uint8_t m_speed;
  uint16_t m_hue;
  uint16_t m_saturation;
  uint16_t m_brightness;
  unsigned int m_offset;
    
public:

  static const int Length = ChimeLength;
  
  Chime()
    : m_pos(0),
      m_speed(128),
      m_hue(210),
      m_saturation(255),
      m_brightness(255),
      m_offset(0) {}

  void setSaturation(uint8_t i) {
    m_saturation = i % 255;
  }
  
  void setSpeed(uint8_t i) {
    m_speed = i % 255;
  }

  void setOffset(unsigned int i) {
    m_offset = i;
  }
  
  void setPos(uint8_t i) {
    m_pos = i;
  }
  
  void setHue(uint8_t i) {
    m_hue = i % 255;
  }

  void setBrightness(uint8_t i) {
    m_brightness = i % 255;
  }
  
  void update() {
    m_pos += 1;
    
    if (random(255) > m_speed) {
      m_pos += 2;
    }
    
    if (m_pos > ChimeLength * 20) {
      m_pos = 0;
      m_hue += 3;
      m_hue %= 255;
    }
  }
  
  void render(Display* dpy) const {
    for(int i = 0; i < ChimeLength; i++) {
      if (i > m_pos) {
        dpy->pixelAt(i + m_offset) = CHSV(0, 0, 0);
      } else {
        uint8_t distance = m_pos - i;
        uint8_t brightness = NSFastLED::scale8(NSFastLED::quadwave8((ChimeLength / (double)distance) * 255), m_brightness);
        if (brightness <= 0.2)
          brightness = 0;
        dpy->pixelAt(VirtualCoordinates{i + m_offset, 0}) = CHSV(m_hue, min(m_saturation, brightness), brightness);
      }
    }
  }
};


