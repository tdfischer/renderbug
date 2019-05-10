#pragma once

class Bounce {
public:
    void attach(int pin, PinMode buttonPinMode) {
        m_pin = pin;
        pinMode(m_pin, buttonPinMode);
        Log.info("Attaching a button to %d", pin);
    }

    void update() {
        int readResult = digitalRead(m_pin);
        if (m_state == Ready) {
            if (readResult == HIGH) {
                m_state = Started;
                m_downStart = millis();
                Log.info("Button %d is started!", m_pin);
            }
        } else if (m_state == Started && millis() - m_downStart >= m_interval) {
            if (readResult == HIGH) {
                m_state = Confirmed;
                Log.info("Button %d is CONFIRMED!", m_pin);
            } else {
                m_state = Ready;
                Log.info("Button %d bounced back to ready!", m_pin);
            }
        } else if (m_state == Confirmed || m_state == Held) {
            if (readResult == LOW) {
                m_state = Ready;
                Log.info("Button %d is released and back to ready!", m_pin);
            } else if (m_state == Confirmed) {
                m_state = Held;
                Log.info("Button %d is being held down!", m_pin);
            }
        }
    }

    void interval(uint8_t v) {
        m_interval = v;
    }

    bool fell() const {
        return m_state == Confirmed;
    }

private:
    enum State {
        Ready,
        Started,
        Confirmed,
        Held
    };

    State m_state = Ready;
    unsigned int m_pin = 0;
    unsigned int m_downStart = 0;
    unsigned int m_interval = 10;
};

class Buttons: public InputSource {
public:
    void onStart() override {
        for(int i = 0; i < 3; i++) {
            m_buttons[i].attach(2 + i, INPUT_PULLDOWN);
            m_buttons[i].interval(15);
        }
    }

    InputEvent read() override {
        for(int i = 0; i < 3; i++) {
            m_buttons[i].update();
            if (m_buttons[i].fell()) {
                return InputEvent{m_buttonMap[i]};
            }
        }
        return InputEvent{};
    }

private:
    Bounce m_buttons[3];
    InputEvent::Intent m_buttonMap[3] = {InputEvent::PowerToggle, InputEvent::NextPattern, InputEvent::UserInput};
};
