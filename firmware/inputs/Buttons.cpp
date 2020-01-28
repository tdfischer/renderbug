#include "Particle.h"
#include "./Buttons.h"

void
Buttons::onStart()
{
    for(int i = 0; i < 3; i++) {
        m_buttons[i].attach(2 + i, INPUT_PULLDOWN);
        m_buttons[i].interval(15);
    }
}

InputEvent
Buttons::read()
{
    for(int i = 0; i < 3; i++) {
        m_buttons[i].update();
        if (m_buttons[i].fell()) {
            return InputEvent{m_buttonMap[i]};
        }
    }
    return InputEvent{};
}
