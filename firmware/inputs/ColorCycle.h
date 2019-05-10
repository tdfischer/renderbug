#include "../Figments/Figments.h"

template<int Period>
class ColorSequenceInput: public InputSource {
public:
    ColorSequenceInput(const std::vector<NSFastLED::CRGB> &colors, const char* name, Task::State initialState)
      : InputSource(name, initialState), m_colors(colors) {}

    InputEvent read() override {
        EVERY_N_SECONDS(Period) {
            m_idx %= m_colors.size();
            return InputEvent{InputEvent::SetColor, m_colors[m_idx++]};
        }
        return InputEvent{};
    }

private:
    std::vector<NSFastLED::CRGB> m_colors;
    int m_idx = 0;
};
