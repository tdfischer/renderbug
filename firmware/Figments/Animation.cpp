#include "./Animation.h"
#include "./Input.h"
#include "./Display.h"
#include <algorithm>

uint8_t
AnimatedNumber::value() const
{
    return NSFastLED::lerp8by8(m_start, m_end, m_idx);
}
