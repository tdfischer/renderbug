#include "./Figment.h"
#include <vector>

class Display;

struct Renderer: public Task {
public:
    Renderer(std::vector<Display*> displays, const std::vector<Figment*> &figments) : Task("Renderer"), m_figments(figments), m_displays(displays) {}

    void loop() override;
    void onStart() override;

private:
    const std::vector<Figment*> m_figments;
    const std::vector<Display*> m_displays;
};
