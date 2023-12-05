#include "GUI.h"

GUI::Element::Element(ElementID id, VEC2 position):
    m_ID(id),
    m_Position(position),
    m_Parent(nullptr)
{
    LOGD("GUI::Element::Element %d", m_ID);
}

GUI::Element::~Element()
{
    LOGD("GUI::Element::~Element %d", m_ID);
}

GUI::Element* GUI::Element::AddChild(Element* element)
{
    element->m_Parent = this;
    m_Children.push_back(element);
    return element;
}

GUI::ContainerElement::ContainerElement(ElementID id, VEC2 position, ContainerProperties properties):
    Element(id, position),
    m_Properties(properties)
{
}

GUI::TextElement::TextElement(ElementID id, VEC2 position, const std::string& text, TextProperties properties):
    Element(id, position),
    m_Text(text),
    m_Properties(properties),
    m_TextObject(nullptr)
{
    LOGD("GUI::Element::TextElement");
}

GUI::TextElement::~TextElement()
{
    LOGD("GUI::Element::~TextElement");
}

void GUI::TextElement::SetTextObject(Text* textObject)
{
    m_TextObject = textObject;
    m_Scale = m_Properties.Size / m_TextObject->GetFont()->GetMaxHeight();
}

glm::mat4 GUI::TextElement::GetTransform()
{
    // TODO: use parents
    // TODO: don't recompute
    VEC3 position(m_Position.x, m_Position.y, 0.f);
    if (m_Properties.Anchor == AnchorType::BOTTOM_RIGHT) {
        position.x -= GetWidth();
    } else if (m_Properties.Anchor == AnchorType::TOP_LEFT) {
        position.y -= GetSize();
    } else if (m_Properties.Anchor == AnchorType::TOP_RIGHT) {
        position.x -= GetWidth();
        position.y -= GetSize();
    }
    glm::mat4 result = glm::translate(MAT4(1.f), position);
    result = glm::scale(result, VEC3(m_Scale, m_Scale, 1.f));
    return result;
}

GUI::GUI::GUI():
    GUI::GUI(0, 0)
{
}

GUI::GUI::GUI(float width, float height):
    m_MaxElementID(0),
    m_Root(CreateContainerElement(VEC2(0, 0), ContainerProperties{ width, height }))
{
    LOGD("GUI::GUI");
}

GUI::GUI::~GUI()
{
    std::queue<Element*> queue;
    queue.push(m_Root);

    while (!queue.empty())
    {
        Element* current = queue.front();
        queue.pop();
        for (Element* child : current->GetChildren())
        {
            queue.push(child);
        }

        // TODO: remove from renderer
        delete current;
        current = nullptr;
    }

    LOGD("GUI::~GUI");
}

GUI::Element* GUI::GUI::AddElement(Element* element)
{
    return m_Root->AddChild(element);
}

void GUI::GUI::UpdateViewSize(int width, int height)
{
    if (m_Root != nullptr)
    {
         // TODO:
    }
}
