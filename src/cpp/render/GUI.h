#pragma once

#include <vector>
#include <queue>

#include "util/Math.h"
#include "util/Logger.h"

namespace GUI
{

using ElementID = std::size_t;

struct TextProperties
{
    enum class FloatType
    {
        LEFT,
        RIGHT
    };

    FloatType Type;
    short Size;
    VEC4 Color;
};

class Element
{
public:
    Element(VEC2 position);
    ~Element();

    Element* AddChild(Element* element);
    const std::vector<Element*>& GetChildren() { return m_Children; }
    ElementID GetID() { return m_ID; }
protected:
    Element* m_Parent;
    std::vector<Element*> m_Children;
    VEC2 m_Position;
private:
    static ElementID s_MaxID;

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

class GUI
{
public:
    GUI();
    // TODO: fix memory leak
    ~GUI() { LOGD("GUI::~GUI"); }

    GUIIterator begin() { return GUIIterator(m_Root); }
    GUIIterator end() { return GUIIterator(); }
    Element* AddElement(Element* element);
private:
    Element* m_Root;
};

class TextElement : public Element
{
public:
    TextElement(const std::string& text, VEC2 position, TextProperties properties);
    ~TextElement();
private:
    std::string m_Text;
    TextProperties m_Properties;
};

}