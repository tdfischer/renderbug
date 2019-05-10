#include "./UpdateStatus.h"
#include "FastLED/FastLED.h"
#include "../Static.h"

using NSFastLED::CRGB;

void
UpdateStatus::handleEvent(const InputEvent& evt)
{
    if (evt.intent == InputEvent::FirmwareUpdate) {
        static int updateCount = 0;
        updateCount++;
        Log.info("Update count %d", updateCount);
        m_updateReady = true;
    }
}

void
UpdateStatus::loop()
{
    m_pos++;
}

void
UpdateStatus::render(Display* dpy) const
{
    if (m_updateReady) {
        for(int i = 0; i < 12; i+=3) {
            dpy->pixelAt(m_pos + i) = CRGB(255, 0, 0);
            dpy->pixelAt(m_pos + i + 1) = CRGB(0, 255, 0);
            dpy->pixelAt(m_pos + i + 2) = CRGB(0, 0, 255);
        }
    }
}

STATIC_ALLOC(UpdateStatus);
