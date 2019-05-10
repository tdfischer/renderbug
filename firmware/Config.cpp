#include "./Config.h"
#include "./Static.h"

HardwareConfig
HardwareConfig::load() {
    HardwareConfig ret;
    EEPROM.get(0, ret);
    return ret;
}

void
HardwareConfig::save() {
    HardwareConfig dataCopy{*this};
    dataCopy.checksum = getCRC();
    EEPROM.put(0, dataCopy);
}

bool
HardwareConfig::isValid() const
{
    return version == 1 && checksum == getCRC();
}

uint8_t
HardwareConfig::getCRC() const
{
  const unsigned char* message = reinterpret_cast<const unsigned char*>(&data);
  constexpr uint8_t length = sizeof(data);
  unsigned char i, j, crc = 0;
  for(i = 0; i < length; i++) {
      crc ^= message[i];
      for(j = 0; j < 8; j++) {
          if (crc & 1) {
              crc ^= CRC7_POLY;
          }
          crc >>= 1;
      }
  }
  return crc;
}

void
ConfigService::onStart()
{
    Log.info("Starting configuration...");
    m_config = HardwareConfig::load();
    if (m_config.isValid()) {
        Log.info("Configuration found!");
    } else {
        Log.info("No configuration found. Writing defaults...");
        m_config = HardwareConfig{};
        m_config.save();
    }
    m_pixelCount = AnimatedNumber{m_config.data.pixelCount};
    m_startPixel = AnimatedNumber{m_config.data.startPixel};
    Log.info("Configured to use %d pixels, starting at %d", m_pixelCount.value(), m_startPixel.value());
    Log.info("Loading task states...");
    for(int i = 0; i < 32; i++) {
        auto svc = m_config.data.serviceStates[i];
        if (strlen(svc.name) > 0) {
            Log.info("* %s: %s", svc.name, svc.isRunning ? "RUNNING" : "STOPPED");
        }
    }
}

void
ConfigService::onConnected()
{
    Log.info("Connecting photon configuration...");
    Particle.function("pixelCount", &ConfigService::setPixelCount, this);
    Particle.function("startPixel", &ConfigService::setStartPixel, this);

    Particle.function("save", &ConfigService::photonSave, this);
    Particle.variable("pixelCount", m_pixelCountInt);
    Particle.variable("startPixel", m_startPixelInt);

    publishConfig();
}

void
ConfigService::loop()
{
    m_startPixel.update();
    m_pixelCount.update();
    m_coordMap.startPixel = m_startPixel;
    m_coordMap.pixelCount = m_pixelCount;
}

void
ConfigService::handleEvent(const InputEvent &evt)
{
    if (evt.intent == InputEvent::NetworkStatus) {
        onConnected();
    }
}

void
ConfigService::publishConfig() const
{
    char buf[255];
    snprintf(buf, sizeof(buf), "{\"pixels\": \"%d\", \"offset\": \"%d\"}", m_config.data.pixelCount, m_config.data.startPixel);
    Particle.publish("renderbug/config", buf, PRIVATE);
}

int
ConfigService::photonSave(String command)
{
    m_config.save();
    return 0;
}

int
ConfigService::setPixelCount(String command)
{
    m_config.data.pixelCount = command.toInt();
    m_pixelCount = m_config.data.pixelCount;
    publishConfig();
    return 0;
}

int
ConfigService::setStartPixel(String command)
{
    m_config.data.startPixel = command.toInt();
    m_startPixel = m_config.data.startPixel;
    publishConfig();
    return 0;
}

STATIC_ALLOC(ConfigService);
