#include "Particle.h"
#include "../Figments/Figments.h"

class PhotonInput: public BufferedInputSource {
public:
    PhotonInput() : BufferedInputSource("PhotonInput") {}
    void onStart() override;
    void handleEvent(const InputEvent &evt) override;

private:
    char m_commandBuf[16];

    void onConnected();
    int reboot(String command);
    int input(String command);
    int setPattern(String command);
    int setHue(String colorName);
    int nextPattern(String command);
    int previousPattern(String command);
    int setPower(String command);
    int setBrightness(String command);

    int startThing(String command);
    int stopThing(String command);

    static void onReset(system_event_t event, int param);
    static void onButtonClick(system_event_t event, int param);
    static void onFirmwareUpdate(system_event_t event, int param);
};
