#pragma once

#include "../Figments/Figments.h"
#include "../sprites/Blob.h"

using NSFastLED::CHSV;

class Flashlight: public Figment {
public:
    Flashlight(Task::State initialState) : Figment("Flashlight", initialState) {
      m_blobs.forEach([](Blob &blob) {
          blob.setHue(random(255));
          blob.setSaturation(10);
          blob.setPos(random(255));
      });
    }

    void handleEvent(const InputEvent& evt) override {
      if (evt.intent == InputEvent::Acceleration) {
          if (evt.asInt() > 10) {
              m_blobs.forEach([](Blob& blob) {blob.update();});
          }
      }

      /*if (evt.intent() == InputEvent::UserInput) {
          if (evt.asInt() == 1) {
              m_blobs.forEach([](Blob& blob) {blob.setPos(random(255));});
          }
          if (evt.asInt() == 2) {
              m_blobs.forEach([](Blob& chime) {blob.setPos(0);});
          }
      }*/
    }

    void loop() override {
      m_blobs.update();
    }

    void render(Display* dpy) const override {
      m_blobs.render(dpy);
      for(int i = 0; i < dpy->pixelCount();i++) {
          dpy->pixelAt(i) |= 100;
      }
    }

private:
    static constexpr int blobCount = 30;
    SpriteList<Blob, blobCount> m_blobs;
};
