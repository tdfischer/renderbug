#include "./MainLoop.h"
#include "./Input.h"
#include "./Figment.h"

void
MainLoop::dispatch(const InputEvent& evt)
{
    if (evt.intent == InputEvent::None) {
        return;
    }
    m_eventBuf.insert(evt);
}

void
MainLoop::loop()
{
    InputEvent evt;
    while (m_eventBuf.take(evt)) {
        if (evt.intent == InputEvent::StartThing || evt.intent == InputEvent::StopThing) {
            const bool jobState = (evt.intent == InputEvent::StartThing);
            for(auto figmentJob: scheduler.tasks) {
                if (strcmp(figmentJob->name, evt.asString()) == 0) {
                    if (jobState) {
                        figmentJob->start();
                    } else {
                        figmentJob->stop();
                    }
                }
            }
        }

        for(Task* task : scheduler) {
            task->handleEvent(evt);
        }
    }
    for(Task* task : scheduler) {
        task->loop();
    }
}

void
MainLoop::start()
{
    Log.info("*** Starting %d tasks...", scheduler.tasks.size());
    Serial.flush();
    for(auto task: scheduler) {
        task->start();
    }
}

MainLoop* MainLoop::s_instance;
