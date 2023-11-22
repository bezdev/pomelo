#pragma once

#include "util/Math.h"

namespace GUI
{

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
    Element();
protected:
    Element* m_Parent;
    VEC2 m_Position;
};

class GUI
{
public:
    GUI();

private:
    Element m_RootElement;
};

class TextElement : Element
{
public:
    TextElement(const std::string& text, TextProperties properties);

};

}