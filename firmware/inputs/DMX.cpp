#if 0
#include "Particle.h"
#include "../Input.h"
#include "../colors.h"
#include "../Static.h"

volatile uint8_t DmxRxField[8];
volatile uint16_t DmxAddress;
enum { IDLE, BREAK, STARTB, STARTADR };

volatile uint8_t gDmxState;

class DMXInput: public InputSource {
public:
    void init() {
        UCSR1B = (1<<RXEN1) | (1<<RXCE1);
        Serial1.begin(250000, SERIAL_8N2);
    }

    static void handleInterrupt() {
        static uint16_t DmxCount;
        uint8_t USARTstate = UCSR1A;
        uint8_t DmxByte = UDR1;
        uint8_t DmxState = gDmxState;

        if (USARTstate &(1<<FE1)) {
            DmxCount = DmxAddress;
            gDmxState = BREAK;
        } else if (DmxState == BREAK) {
            if (DmxByte == 0) {
                gDmxState = STARTB;
            } else {
                gDmxState = IDLE;
            }
        } else if (DmxState == STARTB) {
            if (--DmxCount == 0) {
                DmxCount = 1;
                DmxRxField[0] = DmxByte;
                gDmxState = STARTADR;
            }
        } else if (DmxState == STARTADR) {
            DmxRxField[DmxCount++] = DmxByte;
            if (DmxCount >= sizeof(DmxRxField)) {
                gDmxState = IDLE;
            }
        }
    }

    virtual void update() override {
        bool hasUpdate = false;
        if (gDmxState == IDLE) {
            for(int i = 0; i < 8; i++) {
                if (m_lastReadFields[i] != DmxRxField[i]) {
                    hasUpdate = true;
                    m_lastReadFields[i] = DmxRxField[i];
                }
            }
        }
        if (hasUpdate) {
            m_lastReadEvent = InputEvent(InputEvent::SetColor, {m_lastReadFields[0], m_lastReadFields[1], m_lastReadFields[2]});
        }
        m_lastEvent = m_lastReadEvent;
        m_lastReadEvent = InputEvent();
    }

    virtual InputEvent read() const override {
        return m_lastEvent;
    }

private:
    InputEvent m_lastEvent;
    InputEvent m_lastReadEvent;
    uint8_t m_lastReadFields[8];
};

ISR(USART1_RX_vect) {
    DMXInput::handleInterrupt();
}
#endif
