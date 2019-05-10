#include "../Figments/Figments.h"
#include "../sprites/Blob.h"

using NSFastLED::CRGB;
using NSFastLED::CHSV;
using namespace NSFastLED;

class SolidAnimation: public Figment {
private:
    AnimatedNumber m_red, m_green, m_blue;
    static constexpr int blobCount = 20;
    SpriteList<Blob, blobCount> m_blobs;

public:
    SolidAnimation(Task::State initialState) : Figment("Solid", initialState) {
        m_blobs.forEach([](Blob& blob) {
            blob.setPos(random(140));
            blob.setBrightness(random(255));
            if (random(255) % 2) {
              blob.setVelocity(-1);
            }
        });
    }

    void handleEvent(const InputEvent& evt) override {
        if (evt.intent == InputEvent::SetColor) {
            CRGB nextColor = evt.asRGB();
            m_red.set(nextColor.red);
            m_green.set(nextColor.green);
            m_blue.set(nextColor.blue);
        }
    }

    void loop() override {
        m_red.update();
        m_green.update();
        m_blue.update();
        EVERY_N_MILLIS(6) {
            CRGB rgb{m_red, m_green, m_blue};
            CHSV hsv = NSFastLED::rgb2hsv_approximate(rgb);
            m_blobs.forEach([=](Blob& blob) {
                blob.setHue(hsv.hue);
                blob.setSaturation(hsv.saturation);
            });
            m_blobs.update();
        }
    }

    void render(Display* dpy) const override {
        CRGB color(m_red.value(), m_green.value(), m_blue.value());
        Surface(dpy, {0, 0}, {255, 0}) = color;
        m_blobs.render(dpy);
    }
};
