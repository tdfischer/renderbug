#include "./TestAnimation.h"
#include "FastLED/FastLED.h"

using NSFastLED::CHSV;
using NSFastLED::scale8;

const char*
TestAnimation::name() const 
{
    return "Test";
}

void
TestAnimation::handleEvent(const InputEvent& evt)
{
    if (evt.intent == InputEvent::Acceleration) {
        if (evt.asInt() > 5) {
            m_brightness += 15;
        }
        m_hue += scale8(evt.asInt(), 128);
    }

    if (evt.intent == InputEvent::UserInput) {
      switch(evt.asInt()) {
        case 1:
          m_brightness.set(255, 0);break;
        case 2:
          m_saturation.set(255, 128);break;
        default:
          m_brightness.set(255, 0);
          m_saturation.set(255, 128);
      }
    }
}

void
TestAnimation::loop()
{
    m_x += 4;
    if (m_x % 12 == 0) {
      m_y += 28;
    }
    m_hue.update();
    m_saturation.update();
    m_brightness.update();
}

void
TestAnimation::render(Display* dpy) const
{
    for(uint8_t col = 0; col < 3; col++) {
        for (uint8_t i = 0; i < 254; i+=10) {
            dpy->pixelAt(VirtualCoordinates{(uint8_t)(m_x + (col * (254 / 3))), (uint8_t)(i + m_y)}) = CHSV(m_hue, m_saturation + 100, scale8(i, m_brightness));
        }
    }
}
