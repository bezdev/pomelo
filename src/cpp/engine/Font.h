#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "engine/Components.h"
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
    void CreateFromFile(const char* filename);
    void AddGlyph(char c, Glyph glyph);
    const Glyph& GetGlyph(char c) { return m_Glyphs[c]; }
    float GetMaxHeight() { return m_MaxHeight; }

private:
    std::map<char, Glyph> m_Glyphs;
    float m_MaxHeight;
};

class FontManager
{
public:
    static FontManager* GetInstance()
    {
        if (!s_Instance) s_Instance = new FontManager();
        return s_Instance;
    }

    static void DestroyInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    FontManager() {};
    ~FontManager()
    {
        for (const auto& pair : m_Fonts)
        {
            delete pair.second;
        }
        m_Fonts.clear();
    }

    Font* CreateFont(const char* filename)
    {
        if (m_Fonts.find(filename) != m_Fonts.end()) return m_Fonts[filename];

        Font* f = new Font();
        f->CreateFromFile(filename);
        m_Fonts[filename] = f;

        return f;
    }
private:
    static FontManager* s_Instance;

    std::map<const char*, Font*> m_Fonts;
};
