#include "GUI.h"

GUI::ElementID GUI::Element::s_MaxID = 0;

GUI::Element::Element(VEC2 position):
    m_ID(s_MaxID++),
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

GUI::TextElement::TextElement(const std::string& text, VEC2 position, TextProperties properties):
    Element(position),
    m_Text(text),
    m_Properties(properties)
{
    LOGD("GUI::Element::TextElement");
}

GUI::TextElement::~TextElement()
{
    LOGD("GUI::Element::~TextElement");
}

GUI::GUI::GUI():
    m_Root(new Element(VEC2(0, 0)))
{
    LOGD("GUI::GUI");
}

GUI::Element* GUI::GUI::AddElement(Element *element)
{
    return m_Root->AddChild(element);
}
