#pragma once

#include <vector>
#include <memory>
#include <queue>

#include "render/Text.h"
#include "util/Logger.h"
#include "util/Math.h"

namespace GUI
{

using ElementID = std::size_t;

enum class AnchorType
{
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

struct ContainerProperties
{
    float Width;
    float Height;
};

struct TextProperties
{
    AnchorType Anchor;
    short Size;
    VEC4 Color;
};

class GUI;

class Element
{
public:
    Element(ElementID id, VEC2 position);
    ~Element();

    Element* AddChild(Element* element);

    Element* GetParent() { return m_Parent; };
    const std::vector<Element*>& GetChildren() { return m_Children; }
    ElementID GetID() { return m_ID; }
protected:
    Element* m_Parent;
    std::vector<Element*> m_Children;
    VEC2 m_Position;
private:
    Element() {};

    ElementID m_ID;
};

class GUIIterator : public std::iterator<std::input_iterator_tag, Element>
{
public:
    GUIIterator(Element* root = nullptr)
    {
        if (root)
        {
            m_Queue.push(root);
        }
    }

    GUIIterator& operator++()
    {
        if (!m_Queue.empty())
        {
            Element* current = m_Queue.front();
            m_Queue.pop();
            for (Element* child : current->GetChildren())
            {
                m_Queue.push(child);
            }
        }

        return *this;
    }

    GUIIterator operator++(int)
    {
        GUIIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const GUIIterator& other) const
    {
        return (m_Queue.empty() && other.m_Queue.empty())
            || (!m_Queue.empty() && !other.m_Queue.empty() && m_Queue.front() == other.m_Queue.front());
    }

    bool operator!=(const GUIIterator& other) const
    {
        return !(*this == other);
    }

    Element* operator*()
    {
        return m_Queue.front();
    }
private:
    std::queue<Element*> m_Queue;
};

class ContainerElement: public Element
{
public:
    ContainerElement(ElementID id, VEC2 position, ContainerProperties properties);
private:
    ContainerProperties m_Properties;
};

class TextElement : public Element
{
public:
    TextElement(ElementID id, VEC2 position, const std::string& text, TextProperties properties);
    ~TextElement();

    void SetTextObject(Text* textObject);

    const std::string& GetText() const { return m_Text; }
    Text* GetTextObject() { return m_TextObject; }
    short GetSize() { return m_Properties.Size; }
    float GetWidth() { return m_TextObject->GetWidth() * m_Scale; }
    glm::mat4 GetTransform();
private:
    std::string m_Text;
    TextProperties m_Properties;
    Text* m_TextObject;
    float m_Scale;
};

class GUI
{
public:
    GUI();
    GUI(float width, float height);
    ~GUI();

    GUIIterator begin() { return GUIIterator(m_Root); }
    GUIIterator end() { return GUIIterator(); }
    Element* AddElement(Element* element);

    ContainerElement* CreateContainerElement(VEC2 position, ContainerProperties properties) { return new ContainerElement(m_MaxElementID++, position, properties); }
    TextElement* CreateTextElement(VEC2 position, const std::string& text, TextProperties properties) { return new TextElement(m_MaxElementID++, position, text, properties); }

    Element* GetRoot() { return m_Root; }
private:
    ElementID m_MaxElementID;
    Element* m_Root;
};

}