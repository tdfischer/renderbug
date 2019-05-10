#include "../Figments/Figments.h"

template<uint8_t MaxBrightness = 255, uint32_t MaxMilliAmps = 500, uint32_t Voltage = 5>
class Power: public Figment {
public:
    Power() : Figment("Power") {}

    void handleEvent(const InputEvent& evt) override {
        switch (evt.intent) {
            case InputEvent::PowerToggle:
              m_powerState = m_powerState.value() <= 128 ? 255 : 0;
              Log.info("POWER TOGGLE %d", m_powerState.value());
              break;
            case InputEvent::SetPower:
              m_powerState = evt.asInt() == 0 ? 0 : 255;
              break;
            case InputEvent::SetBrightness:
              m_brightness = evt.asInt();
        }
    }

    void loop() override {
        m_powerState.update();
        m_brightness.update();
    }

    void render(Display* dpy) const override {
        const uint8_t clippedBrightness = min((uint8_t)m_brightness, MaxBrightness);
        const uint8_t scaledBrightness = NSFastLED::scale8(m_powerState, clippedBrightness);
        const uint8_t videoBrightness = NSFastLED::brighten8_video(scaledBrightness);
        const uint8_t powerBrightness = NSFastLED::calculate_max_brightness_for_power_mW(videoBrightness, Watts);
        NSFastLED::FastLED.setBrightness(powerBrightness);
    }

    static constexpr uint32_t Watts = Voltage * MaxMilliAmps;

private:
    AnimatedNumber m_powerState = 255;
    AnimatedNumber m_brightness = MaxBrightness;
};
