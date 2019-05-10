#include "CloudStatus.h"
#include "../Static.h"

void
CloudStatus::onStart()
{
    SINGLE_THREADED_BLOCK() {
        if (Particle.connected()) {
            initNetwork(0, cloud_status_connected);
        } else {
            System.on(cloud_status, &CloudStatus::initNetwork);
        }
    }
}


void
CloudStatus::initNetwork(system_event_t event, int param) {
    if (param == cloud_status_connected) {
        Log.info("Connected to T H E  C L O U D");
        MainLoop::instance()->dispatch(InputEvent{InputEvent::NetworkStatus, true});
    }
}

STATIC_ALLOC(CloudStatus);
