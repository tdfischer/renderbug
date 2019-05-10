#include "Particle.h"
#include "../Figments/Figments.h"

class SerialInput: public InputSource {
public:
    void onAttach() override {
        //Serial.begin();
    }

    InputEvent read() {
    }
}
