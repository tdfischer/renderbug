#include "PhotonTelemetry.h"

using namespace NSFastLED;

PhotonTelemetry::PhotonTelemetry() : Task("PhotonTelemetry") {}

void
PhotonTelemetry::onConnected()
{
    Log.info("Connecting photon telemetry...");
    Particle.variable("frame", m_frameIdx);
    Particle.variable("brightness", m_currentBrightness);
    Particle.variable("fps", m_fps);
    Particle.variable("services", m_serviceList);
    m_online = true;
}

void
PhotonTelemetry::loop()
{
    m_frameIdx++;
    if (m_rgbPulseFrame == 1) {
        m_ledStatus.setActive(false);
    } else if (m_rgbPulseFrame > 0) {
        m_rgbPulseFrame--;
    }

    m_currentBrightness = NSFastLED::FastLED.getBrightness();
    NSFastLED::FastLED.countFPS();
    m_fps = NSFastLED::FastLED.getFPS();

    if (m_online) {
        EVERY_N_SECONDS(30) {
            char valueBuf[255];
            snprintf(valueBuf, sizeof(valueBuf), "{\"fps\": %lu}", m_fps);
            Log.info("Heartbeat: %s", valueBuf);
            Particle.publish("renderbug/heartbeat", valueBuf);
            auto sched = MainLoop::instance()->scheduler;
            m_serviceList = String{};
            for(auto task : sched.tasks) {
                m_serviceList.concat(task->name);
                m_serviceList.concat(':');
                if (task->state == Task::Running) {
                    m_serviceList.concat(1);
                } else {
                    m_serviceList.concat(0);
                }
                m_serviceList.concat(',');
            }
        }
    }
}

void
PhotonTelemetry::handleEvent(const InputEvent &evt)
{
    m_lastEvent = evt;
    if (evt.intent == InputEvent::NetworkStatus) {
        onConnected();
    }
    if (evt.intent != InputEvent::None) {
        const char* sourceName;
        switch(evt.intent) {
            case InputEvent::PowerToggle:
              sourceName = "power-toggle";
              break;
            case InputEvent::SetPower:
              sourceName = "set-power";
              break;
            case InputEvent::PreviousPattern:
              sourceName = "previous-pattern";
              break;
            case InputEvent::NextPattern:
              sourceName = "next-pattern";
              break;
            case InputEvent::SetPattern:
              sourceName = "set-pattern";
              break;
            case InputEvent::SetColor:
              sourceName = "set-color";
              break;
            case InputEvent::Acceleration:
              sourceName = "acceleration";
              break;
            case InputEvent::UserInput:
              sourceName = "user";
              break;
            case InputEvent::SetBrightness:
              sourceName = "set-brightness";
              break;
            case InputEvent::FirmwareUpdate:
              sourceName = "firmware-update";
              break;
            case InputEvent::NetworkStatus:
              sourceName = "network-status";
              break;
            case InputEvent::StartThing:
              sourceName = "start-thing";
              break;
            case InputEvent::StopThing:
              sourceName = "stop-thing";
              break;
            default:
              sourceName = "unknown";
              break;
        }
        char valueBuf[255];
        switch(evt.type) {
            case InputEvent::Null:
              snprintf(valueBuf, sizeof(valueBuf), "null");break;
            case InputEvent::Integer:
              snprintf(valueBuf, sizeof(valueBuf), "%d", evt.asInt());break;
            case InputEvent::String:
              snprintf(valueBuf, sizeof(valueBuf), "\"%s\"", evt.asString());break;
            case InputEvent::Color:
              snprintf(valueBuf, sizeof(valueBuf), "[%d, %d, %d]", evt.asRGB().r, evt.asRGB().g, evt.asRGB().b);break;
        }
        char buf[255];
        snprintf(buf, sizeof(buf), "{\"intent\": \"%s\", \"value\": %s}", sourceName, valueBuf);
        if (m_online) {
            Log.info("Event: %s", buf);
            Particle.publish("renderbug/event", buf, PRIVATE);
        } else {
            Log.info("[offline] Event: %s", buf);
        }

        if (evt.intent == InputEvent::SetColor) {
            NSFastLED::CRGB rgb {evt.asRGB()};
            uint32_t color = (rgb.r << 16) + (rgb.g << 8) + (rgb.b << 0);
            m_ledStatus.setColor(color);
            m_ledStatus.setActive(true);
            m_rgbPulseFrame = 1000;
        }
    }
}
