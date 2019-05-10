#pragma once
#include "FastLED/FastLED.h"
#include "./Figment.h"
#include <vector>

class Display;

struct AnimatedNumber {
    void set(uint8_t v) {
        set(value(), v);
    }

    void set(uint8_t start, uint8_t end) {
        m_start = start;
        m_end = end;
        m_idx = 0;
    }

    uint8_t value() const;

    void update() {
        if (m_idx < 255) {
            m_idx += 1;
        }
    }

    AnimatedNumber() {}
    AnimatedNumber(uint8_t v) : m_end(v) {}

    AnimatedNumber& operator=(uint8_t v) {
        set(v);
        return *this;
    }

    AnimatedNumber& operator+=(uint8_t v) {
        set(value()+v);
        return *this;
    }

    operator uint8_t() const {
        return value();
    }

    operator int() const {
        return value();
    }

    operator unsigned int() const {
        return value();
    }

    bool operator==(int other) const {
        return value() == other;
    }

    uint8_t operator+(uint8_t other) const {
        return value() + other;
    }

    int operator+(int other) const {
        return value() + other;
    }

private:
    uint8_t m_idx = 255;
    uint8_t m_start = 0;
    uint8_t m_end = 0;
};

struct AnimatedRGB {
    NSFastLED::CRGB start;
    NSFastLED::CRGB end;
    AnimatedNumber pos;

    AnimatedRGB(const NSFastLED::CRGB& color)
      : start(color), end(color) {}

    AnimatedRGB() {}

    AnimatedRGB& operator=(const NSFastLED::CRGB& rgb) {
        start = *this;
        end = rgb;
        pos.set(0, 255);
        return *this;
    }

    void update() {
        pos.update();
    }

    operator NSFastLED::CRGB() const {
        uint8_t red = NSFastLED::lerp8by8(start.red, end.red, pos);
        uint8_t green = NSFastLED::lerp8by8(start.green, end.green, pos);
        uint8_t blue = NSFastLED::lerp8by8(start.blue, end.blue, pos);
        return NSFastLED::CRGB(red, green, blue);
    }
};

template<typename T, int Size>
struct SpriteList {
    void update() {
        if (!m_enabled) return;
        for(int i = 0; i < size; i++) {
            animations[i].update();
        }
    }

    void render(Display* dpy) const {
        if (!m_enabled) return;
        for(int i = 0; i < size; i++) {
            animations[i].render(dpy);
        }
    }

    void forEach(std::function<void(T&)> func) {
        for(int i = 0; i < size; i++) {
            func(animations[i]);
        }
    }

    void forEach(std::function<void(const T&)> func) const {
        for(int i = 0; i < size; i++) {
            func(animations[i]);
        }
    }

    void disable() {
        m_enabled = false;
    }

    void enable() {
        m_enabled = true;
    }

    void toggle() {
        m_enabled = !m_enabled;
    }

    T& operator[](int idx) {
        return animations[idx];
    }

    T animations[Size];
    bool m_enabled = true;
    static constexpr int size = Size;
    using Type = T;
};
