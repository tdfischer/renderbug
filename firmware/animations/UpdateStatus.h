#include "../Figments/Figments.h"

class UpdateStatus: public Figment {
public:
    UpdateStatus() : Figment("UpdateStatusAnimation") {}
    void handleEvent(const InputEvent& evt) override;
    void loop() override;
    void render(Display* dpy) const override;

private:
    bool m_updateReady = false;
    uint8_t m_pos = 0;
};
