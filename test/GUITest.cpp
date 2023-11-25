#include "Test.h"

#include <memory>

#include "render/GUI.h"
#include "util/Logger.h"
#include "util/Color.h"

TEST(BasicGUITest)
{
    std::shared_ptr<GUI::GUI> gui(std::make_shared<GUI::GUI>());

    auto e1 = gui->AddElement(new GUI::TextElement(
        std::string("fps"),
        VEC2(10, 10),
        GUI::TextProperties {
            GUI::TextProperties::FloatType::RIGHT,
            10,
            V_COLOR_RED
        }));

    auto e2 = e1->AddChild(new GUI::TextElement(
        std::string("fps"),
        VEC2(10, 10),
        GUI::TextProperties {
            GUI::TextProperties::FloatType::RIGHT,
            10,
            V_COLOR_RED
        }));

    auto e3 = gui->AddElement(new GUI::TextElement(
        std::string("fps"),
        VEC2(10, 10),
        GUI::TextProperties {
            GUI::TextProperties::FloatType::RIGHT,
            10,
            V_COLOR_RED
        }));

    std::vector<GUI::ElementID> expected = {
        0, 1, 3, 2
    };
    size_t i = 0;
    for (GUI::Element* e : *gui)
    {
        LOGT("id: %d", e->GetID());
        ASSERT_ARE_EQUAL(e->GetID(), expected[i++]);
    }

    // LOGT("%d", expected[500000000]);
}
