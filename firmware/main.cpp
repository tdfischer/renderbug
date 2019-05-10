#include "FastLED/FastLED.h"
#include "Figments/Figments.h"

#include "PhotonTelemetry.h"
#include "Static.h"
#include "Config.h"
#include "colors.h"

#include "animations/Power.cpp"
#include "animations/SolidAnimation.cpp"
#include "animations/Chimes.cpp"
#include "animations/Flashlight.cpp"
#include "animations/UpdateStatus.h"

#include "inputs/Photon.h"
#include "inputs/ColorCycle.h"
#include "inputs/CloudStatus.h"
#include "inputs/MPU6050.h"
#include "inputs/Buttons.h"

SerialLogHandler logHandler;

using namespace NSFastLED;

#define LED_NUM 256
#define MAX_BRIGHTNESS 255
#define PSU_MILLIAMPS 4800

// Enable system thread, so rendering happens while booting
SYSTEM_THREAD(ENABLED);

// Setup FastLED and the display
CRGB leds[LED_NUM];
Display dpy(leds, LED_NUM, Static<ConfigService>::instance()->coordMap());

LinearCoordinateMapping neckMap{60, 0};
Display neckDisplay(leds, LED_NUM, &neckMap);

// Setup power management
Power<MAX_BRIGHTNESS, PSU_MILLIAMPS> power;

class DrainAnimation: public Figment {
public:

    DrainAnimation(Task::State initialState) : Figment("Drain", initialState) {}

    void loop() override {
        EVERY_N_MILLISECONDS(15) {
            m_pos++;
            m_fillColor.update();
        }
        EVERY_N_MILLISECONDS(50) {
            m_burst -= m_burst / 10;
        }
    }

    void handleEvent(const InputEvent& event) override {
        if (event.intent == InputEvent::SetColor) {
            m_fillColor = event.asRGB();
        } else if (event.intent == InputEvent::Acceleration) {
            m_pos += log10(event.asInt());
            uint16_t burstInc = event.asInt() / 6;
            m_burst = (m_burst > 0xFFFF - burstInc) ? 0xFFFF : m_burst + burstInc;
        }
    }

    AnimatedRGB m_fillColor;

    void render(Display* dpy) const override {
        dpy->clear();
        Surface leftPanel{dpy, {0, 0}, {128, 0}};
        Surface rightPanel{dpy, {128, 0}, {255, 0}};
        fillRange(dpy, leftPanel.start, leftPanel.end, rgb2hsv_approximate(m_fillColor));
        fillRange(dpy, rightPanel.end, rightPanel.start, rgb2hsv_approximate(m_fillColor));
    }

    void fillRange(Display* dpy, const PhysicalCoordinates &start, const PhysicalCoordinates& end, const CHSV &baseColor) const {
        int length = end.x - start.x;
        int direction = 1;
        if (length < 0) {
            direction = -1;
        }

        uint8_t frac = 255 / std::abs(length);
        for(int i = 0; i < std::abs(length); i++) {
            auto coords = PhysicalCoordinates((start.x + (i * direction)), 0);

            const uint8_t localScale = inoise8(i * 80, m_pos * 3);
            const uint8_t dimPosition = lerp8by8(50, 190, scale8(sin8((frac * i) / 2), localScale));
            const uint8_t withBurst = ease8InOutCubic(lerp16by16(dimPosition, 255, m_burst));
            auto scaledColor = CHSV(baseColor.hue, lerp8by8(100, 255, localScale), withBurst);

            CRGB src(dpy->pixelAt(coords));
            dpy->pixelAt(coords) = blend(scaledColor, src, 200);
        }
    }

    uint16_t m_pos;
    uint16_t m_burst;
};

// Clip the display at whatever is configured while still showing over-paints
FigmentFunc displayClip([](Display* dpy) {
    auto coords = Static<ConfigService>::instance()->coordMap();
    for(int i = 0; i < coords->startPixel; i++) {
        dpy->pixelAt(i) %= 40;
    }
    for(int i = LED_NUM; i > coords->pixelCount + coords->startPixel; i--) {
        dpy->pixelAt(i) %= 40;
    }
});

ChimesAnimation chimes{Task::Stopped};
SolidAnimation solid{Task::Stopped};
DrainAnimation drain{Task::Running};
Flashlight flashlight{Task::Stopped};


// Render all layers to the displays
Renderer renderer{
    //{&dpy, &neckDisplay},
    {&dpy},
    {
        &chimes,
        &drain,
        &solid,
        &flashlight,
        Static<UpdateStatus>::instance(),
        &displayClip,
        &power,
    }
};

// Photon telemetry needs a reference to the selected animation's name, so we
// set it up here
PhotonTelemetry telemetry;

// Cycle some random colors
ColorSequenceInput<7> noisebridgeCycle{{colorForName("Red").rgb}, "Noisebridge", Task::Stopped};
ColorSequenceInput<7> hackerbotsCycle{{colorForName("Purple").rgb}, "Hackerbots", Task::Stopped};

ColorSequenceInput<13> kierynCycle{{
    colorForName("Cerulean").rgb,
    colorForName("Electric Purple").rgb,
    colorForName("Emerald").rgb,
    colorForName("Sky Magenta").rgb
}, "Kieryn", Task::Running};

ColorSequenceInput<7> rainbowCycle{{
    colorForName("Red").rgb,
    colorForName("Orange").rgb,
    colorForName("Yellow").rgb,
    colorForName("Green").rgb,
    colorForName("Blue").rgb,
    colorForName("Purple").rgb,
    colorForName("White").rgb,
}, "Rainbow", Task::Running};

// Turn on,
MainLoop runner{{

    // Load/update graphics configuration from EEPROM and Particle
    Static<ConfigService>::instance(),

    // Particle cloud status
    Static<CloudStatus>::instance(),

    // Monitor network state and provide particle API events
    Static<PhotonInput>::instance(),

    new MPU5060(),
    new Buttons(),

    // Periodic color inputs
    &noisebridgeCycle,
    &hackerbotsCycle,
    &kierynCycle,
    &rainbowCycle,

    // Animations
    &chimes,
    &drain,
    &solid,
    &flashlight,

    // Update UI layer
    &power,
    &displayClip,
    Static<UpdateStatus>::instance(),

    // Render everything
    &renderer,

    // Update photon telemetry
    &telemetry,
}};

// Tune in,
void setup() {
  Serial.begin(115200);
  //while(!Serial.isConnected()) { Particle.process(); }
  //Serial.println("Hello, there!");
  Log.info("🐛 Booting Renderbug %s!", System.deviceID().c_str());
  Log.info("🐞 I am built for %d LEDs running on %dmA", LED_NUM, PSU_MILLIAMPS);
  Log.info("📡 Particle version %s", System.version().c_str());

  Log.info("💡 Starting FastLED...");
  FastLED.addLeds<NEOPIXEL, 6>(leds, LED_NUM);

  Log.info("🌌 Starting Figment...");
  Serial.flush();
  runner.start();

  Log.info("💽 %lu bytes of free RAM", System.freeMemory());
  Log.info("🚀 Setup complete! Ready to rock and roll.");
  Serial.flush();
}

// Drop out.
void loop() {
  runner.loop();
}
