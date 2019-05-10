#include "../Figments/Figments.h"

class CloudStatus: public Task {
public:
    CloudStatus() : Task("Cloud") {}
    void loop() override {}
    void onStart() override;

private:
    static void initNetwork(system_event_t event, int param);
};
