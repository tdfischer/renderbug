#pragma once
#include "application.h"
#include "./Geometry.h"
#include "./Figment.h"
#include "FastLED/FastLED.h"

typedef Vector3d<int> MotionVec;

struct Variant {
    enum Type {
        Null,
        Integer,
        String,
        Color,
    };

    Variant(int v)
      : type(Integer), m_value{.asInt=v} {}

    Variant(const char* v)
      : type(String), m_value{.asString=v} {}

    Variant(const NSFastLED::CRGB &v)
      : type(Color), m_value{.asRGB={v.r, v.g, v.b}} {}

    Variant()
      : type(Null) {}

    Type type;

    const char* asString() const;
    NSFastLED::CRGB asRGB() const;
    int asInt() const;

private:
    union {
        int asInt;
        const char* asString;
        uint8_t asRGB[3];
    } m_value;
};

struct InputEvent: public Variant {
    enum Intent {
        None,
        PowerToggle,
        SetPower,
        SetBrightness,
        PreviousPattern,
        NextPattern,
        SetPattern,
        SetColor,
        Acceleration,
        FirmwareUpdate,
        NetworkStatus,
        StartThing,
        StopThing,
        UserInput,
    };

    template<typename Value>
    InputEvent(Intent s, Value v)
      : Variant(v), intent(s) {}

    InputEvent(Intent s)
      : Variant(), intent(s) {}

    InputEvent()
      : Variant(), intent(None) {}

    Intent intent;
};

class InputSource: public Task {
public:
    InputSource() : Task() {}
    InputSource(const char* name) : Task(name) {}
    InputSource(Task::State initialState) : Task(initialState) {}
    InputSource(const char* name, Task::State initialState) : Task(name, initialState) {}
    void loop() override;
    virtual InputEvent read() = 0;
};

class BufferedInputSource: public InputSource {
public:
    BufferedInputSource() : InputSource() {}
    BufferedInputSource(const char* name) : InputSource(name) {}
    InputEvent read() override;

protected:
    void setEvent(InputEvent &&evt);

private:
    InputEvent m_lastEvent;
};
