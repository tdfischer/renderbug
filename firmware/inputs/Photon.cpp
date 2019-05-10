#include "Particle.h"
#include "../Figments/Figments.h"
#include "../colors.h"
#include "../Static.h"
#include "./Photon.h"

void
PhotonInput::onConnected()
{
    Log.info("Connecting photon input...");
    Particle.function("power", &PhotonInput::setPower, this);
    Particle.function("next", &PhotonInput::nextPattern, this);
    Particle.function("input", &PhotonInput::input, this);
    Particle.function("previous", &PhotonInput::previousPattern, this);
    Particle.function("pattern", &PhotonInput::setPattern, this);
    Particle.function("setHue", &PhotonInput::setHue, this);
    Particle.function("brightness", &PhotonInput::setBrightness, this);
    Particle.function("reboot", &PhotonInput::reboot, this);
    Particle.function("start", &PhotonInput::startThing, this);
    Particle.function("stop", &PhotonInput::stopThing, this);
}

int
PhotonInput::startThing(String command)
{
    command.toCharArray(m_commandBuf, sizeof(m_commandBuf));
    setEvent(InputEvent(InputEvent::StartThing, m_commandBuf));
    return 0;
}

int
PhotonInput::stopThing(String command)
{
    command.toCharArray(m_commandBuf, sizeof(m_commandBuf));
    setEvent(InputEvent(InputEvent::StopThing, m_commandBuf));
    return 0;
}

void
PhotonInput::onStart()
{
    System.on(firmware_update, &PhotonInput::onFirmwareUpdate);
    System.on(button_click, &PhotonInput::onButtonClick);
    System.on(reset, &PhotonInput::onReset);
}

void
PhotonInput::handleEvent(const InputEvent &evt)
{
    if (evt.intent == InputEvent::NetworkStatus) {
        onConnected();
    }
}

int
PhotonInput::reboot(String command)
{
    System.reset();
    return 0;
}

void
PhotonInput::onReset(system_event_t event, int param)
{
    NSFastLED::FastLED.clear();
}

void
PhotonInput::onButtonClick(system_event_t event, int param)
{
    Static<PhotonInput>::instance()->setEvent(InputEvent{InputEvent::NextPattern, param});
}

void
PhotonInput::onFirmwareUpdate(system_event_t event, int param)
{
    Static<PhotonInput>::instance()->setEvent(InputEvent{InputEvent::FirmwareUpdate, param});
}

int
PhotonInput::input(String command)
{
    command.toCharArray(m_commandBuf, sizeof(m_commandBuf));
    setEvent(InputEvent(InputEvent::UserInput, m_commandBuf));
    return 0;
}

int
PhotonInput::setPattern(String command)
{
    command.toCharArray(m_commandBuf, sizeof(m_commandBuf));
    setEvent(InputEvent(InputEvent::SetPattern, m_commandBuf));
    return 0;
}

int
PhotonInput::setHue(String colorName)
{
    ColorInfo nextColor = colorForName(colorName);
    setEvent(InputEvent(InputEvent::SetColor, nextColor.rgb));
    return 0;
}

int
PhotonInput::nextPattern(String command)
{
    setEvent(InputEvent(InputEvent::NextPattern));
    return 0;
}

int
PhotonInput::previousPattern(String command)
{
    setEvent(InputEvent(InputEvent::PreviousPattern));
    return 0;
}

int
PhotonInput::setPower(String command)
{
    if (command == "off") {
        setEvent(InputEvent(InputEvent::SetPower, 0));
    } else if (command == "on") {
        setEvent(InputEvent(InputEvent::SetPower, 1));
    } else {
        setEvent(InputEvent(InputEvent::PowerToggle));
    }
    return 0;
}

int
PhotonInput::setBrightness(String command)
{
    setEvent(InputEvent(InputEvent::SetBrightness, command.toInt()));
    return 0;
}

STATIC_ALLOC(PhotonInput);
