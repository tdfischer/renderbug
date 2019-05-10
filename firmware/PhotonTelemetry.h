#pragma once
#include "./Figments/Figments.h"

class PhotonTelemetry: public Task {
public:
    PhotonTelemetry();
    void loop() override;
    void handleEvent(const InputEvent& evt) override;

private:
    void onConnected();

    int m_frameIdx;
    String m_serviceList;
    InputEvent m_lastEvent;
    uint32_t m_currentBrightness;
    LEDStatus m_ledStatus = LEDStatus(0, LED_PATTERN_FADE, LED_SPEED_FAST);
    uint32_t m_rgbPulseFrame = 0;
    uint32_t m_pixelCount = 0;
    uint32_t m_startPixel = 0;
    uint32_t m_fps = 0;
    bool m_online = false;
};
