#include "../Figments/Figments.h"

class TestAnimation: public Figment {
public:
    const char* name() const;
    void handleEvent(const InputEvent& evt) override;
    void loop() override;
    void render(Display* dpy) const override;

private:
    AnimatedNumber m_hue;
    AnimatedNumber m_saturation;
    AnimatedNumber m_brightness;
    uint8_t m_x;
    uint8_t m_y;
};
