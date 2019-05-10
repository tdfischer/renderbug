#include "application.h"
#include "./Input.h"
#include "./MainLoop.h"

NSFastLED::CRGB
Variant::asRGB() const
{
    return NSFastLED::CRGB(m_value.asRGB[0], m_value.asRGB[1], m_value.asRGB[2]);
}

const char*
Variant::asString() const
{
    return m_value.asString;
}

int
Variant::asInt() const
{
    return m_value.asInt;
}

void
InputSource::loop()
{
    MainLoop::instance()->dispatch(read());
}

InputEvent
BufferedInputSource::read()
{
    InputEvent ret = m_lastEvent;
    m_lastEvent = InputEvent{};
    return ret;
}

void
BufferedInputSource::setEvent(InputEvent &&evt)
{
    m_lastEvent = std::move(evt);
}
