#pragma once
#include "Particle.h"
#include "./Figments/Figments.h"

struct HardwareConfig {
    uint8_t version = 1;
    uint8_t checksum = 0;
    struct TaskState {
        char name[16] = {0};
        bool isRunning = false;
    };
    struct Data {
        uint16_t pixelCount = 255;
        uint16_t startPixel = 0;
        TaskState serviceStates[32];
    };
    Data data;

    static HardwareConfig load();
    void save();
    bool isValid() const;

private:
    uint8_t getCRC() const;

    static constexpr uint8_t CRC7_POLY = 0x91;
};

// A task that manages the EEPROM settings and coord mapping when modified via
// Particle. This allows for multiple devices with wildly different displays to
// run the same code
struct ConfigService: public Task {
    ConfigService() : Task("Configuration") {}
    void onStart();
    void loop() override;
    void handleEvent(const InputEvent &evt) override;
    const LinearCoordinateMapping* coordMap() const { return &m_coordMap; }

private:
    void onConnected();
    void publishConfig() const;
    int photonSave(String command);
    int setPixelCount(String command);
    int setStartPixel(String command);

    AnimatedNumber m_pixelCount;
    AnimatedNumber m_startPixel;
    int m_pixelCountInt;
    int m_startPixelInt;

    HardwareConfig m_config;
    LinearCoordinateMapping m_coordMap;
};
