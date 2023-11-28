#include "Test.h"

#include <memory>

#include "render/GUI.h"
#include "util/Logger.h"
#include "util/Color.h"

namespace GUITest
{
    static std::shared_ptr<GUI::GUI> CreateTestGUI()
    {
        std::shared_ptr<GUI::GUI> gui(std::make_shared<GUI::GUI>());

        auto e1 = gui->AddElement(gui->CreateTextElement(
            VEC2(10, 10),
            std::string("fps"),
            GUI::TextProperties {
                GUI::AnchorType::BOTTOM_LEFT,
                10,
                V_COLOR_RED
            }));

        auto e2 = e1->AddChild(gui->CreateTextElement(
            VEC2(10, 10),
            std::string("fps"),
            GUI::TextProperties {
                GUI::AnchorType::BOTTOM_LEFT,
                10,
                V_COLOR_RED
            }));

        auto e3 = gui->AddElement(gui->CreateTextElement(
            VEC2(10, 10),
            std::string("fps"),
            GUI::TextProperties {
                GUI::AnchorType::BOTTOM_LEFT,
                10,
                V_COLOR_RED
            }));

        return std::move(gui);
    }
};

TEST(BasicGUITest)
{
    auto guiP = GUITest::CreateTestGUI();
    GUI::GUI& gui = *guiP;

    std::vector<GUI::ElementID> expected = {
        0, 1, 3, 2
    };
    size_t i = 0;
    for (GUI::Element* e : gui)
    {
        LOGT("id: %d", e->GetID());
        ASSERT_ARE_EQUAL(e->GetID(), expected[i++]);
    }
}

TEST(BasicGUIParentIteratorTest)
{
    auto guiP = GUITest::CreateTestGUI();
    GUI::GUI& gui = *guiP;

    std::vector<GUI::ElementID> expected = {
        2, 1, 0
    };

    size_t i = 0;
    auto lastChild = gui.GetRoot()->GetChildren()[0]->GetChildren()[0];
    while (lastChild != nullptr)
    {
        LOGT("id: %d", lastChild->GetID());
        ASSERT_ARE_EQUAL(lastChild->GetID(), expected[i++]);
        lastChild = lastChild->GetParent();
    }
}

//TEST_THROW()
