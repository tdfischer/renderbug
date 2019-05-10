#include "../Figments/Figments.h"
#include "../sprites/Chime.h"
#include "../sprites/Blob.h"

using namespace NSFastLED;

#define CHIME_LENGTH 23
#define CHIME_COUNT 4
#define BLOB_COUNT 10

class ChimesAnimation: public Figment {
public:
    ChimesAnimation(Task::State initialState) : Figment("Chimes", initialState) {
      m_chimes.forEach([](Chime<CHIME_LENGTH> &chime) {
        chime.setPos(random(Chime<CHIME_LENGTH>::Length * 5));
        chime.setHue(random(255));
        chime.setSpeed(random(90) + 138);
        chime.setBrightness(200);
        chime.setOffset(random(1024));
      });
      m_blobs.forEach([](Blob &blob) {
        blob.setPos(random(255));
        blob.setHue(random(255));
        blob.setBrightness(random(255));
        if (random(255) % 2) {
          blob.setVelocity(-1);
        } else {  
          blob.setVelocity(1);
        }
      });
    }

    void handleEvent(const InputEvent& evt) override {
        if (evt.intent == InputEvent::UserInput) {
            if (strcmp(evt.asString(), "blobs") == 0) {
                m_blobs.toggle();
            } else if (strcmp(evt.asString(), "chimes") == 0) {
                m_chimes.toggle();
            }
        } else if (evt.intent == InputEvent::SetColor) {
            m_flashBrightness.set(255, 0);
            m_flashColor = evt.asRGB();
            uint8_t flashHue = rgb2hsv_approximate(m_flashColor).hue;
            m_blobs.forEach([&](Blob& blob) {
                blob.setHue(flashHue);
            });
            m_chimes.forEach([&](Chime<CHIME_LENGTH>& chime) {
                chime.setHue(flashHue);
            });
        }
    }

    void loop() override {
      m_chimes.update();
      m_blobs.update();
      m_flashColor.update();
      EVERY_N_MILLISECONDS(5) {
          m_flashBrightness.update();
      }
    }

    void render(Display* dpy) const override {
      m_chimes.render(dpy);
      m_blobs.render(dpy);
      Surface fullSurface(dpy, {0, 0}, {255, 0});
      NSFastLED::CRGB scaledColor = NSFastLED::CRGB(m_flashColor).nscale8_video(max(10, NSFastLED::ease8InOutCubic(m_flashBrightness)));
      fullSurface.paintWith([&](NSFastLED::CRGB& pixel) {
          pixel = NSFastLED::blend(scaledColor, pixel, 200);
          //pixel = scaledColor;
      });
    }

    SpriteList<Chime<CHIME_LENGTH>, CHIME_COUNT> m_chimes;
    SpriteList<Blob, BLOB_COUNT> m_blobs;
    AnimatedRGB m_flashColor;
    AnimatedNumber m_flashBrightness;
};
