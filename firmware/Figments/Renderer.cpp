#include "./Renderer.h"
#include "./Display.h"

void
Renderer::loop()
{
    for(Display* dpy : m_displays) {
        for(Figment* figment : m_figments) {
            if (figment->state == Task::Running) {
                figment->render(dpy);
            }
        };
    }
    NSFastLED::FastLED.show();
}

void
Renderer::onStart()
{
    for(Display* dpy : m_displays) {
        dpy->clear();
    }
    NSFastLED::FastLED.show();
}
