#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "util/Util.h"

struct Glyph
{
    float Advance;
    float BitmapLeft;
    float BitmapTop;
    float Width;
    float Height;
    float u1, v1, u2, v2;
};

class Font
{
public:
    Font();
    void AddGlyph(char c, Glyph glyph);
    void CreateFromFile(const char* filename);

private:
    std::map<char, Glyph> m_Glyphs;
};
