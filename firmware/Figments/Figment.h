#pragma once
#include "application.h"
#include <functional>

class Display;
class InputEvent;
class InputSource;

struct Task {
    virtual void handleEvent(const InputEvent& event) {}
    virtual void loop() = 0;
    virtual void onStart() {};
    virtual void onStop() {};

    enum State {
        Running,
        Stopped,
    };

    Task() {}
    Task(State initialState) : Task(0, initialState) {}
    Task(const char* name) : Task(name, Running) {}
    Task(const char* name, State initialState) : name(name), state(initialState) {}

    void start() { Log.info("* Starting %s...", name); state = Running; onStart(); }
    void stop() { Log.info("* Stopping %s...", name); onStop(); state = Stopped; }

    const char* name = 0;
    State state = Running;
};

struct Figment: public Task {
    Figment() : Task() {}
    Figment(State initialState) : Task(initialState) {}
    Figment(const char* name) : Task(name) {}
    Figment(const char* name, State initialState) : Task(name, initialState) {}
    virtual void render(Display* dpy) const = 0;
};

struct FigmentFunc: public Figment {
    FigmentFunc(std::function<void(Display*)> func) : Figment("lambda"), func(func) {}
    void loop() override {}
    void render(Display* dpy) const override {
        func(dpy);
    }
    std::function<void(Display*)> func;
};
