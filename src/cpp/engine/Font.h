#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "engine/ECS.h"
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

private:
    std::map<char, Glyph> m_Glyphs;
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

    Font* AddFont(Components::FontType fontType)
    {
        if (m_Fonts.find(Components::FontType::DEFAULT) != m_Fonts.end()) return m_Fonts[Components::FontType::DEFAULT];

        Font* f = new Font();
        f->CreateFromFile("assets/fonts/default.csv");
        m_Fonts[Components::FontType::DEFAULT] = f;

        return f;
    }
private:
    static FontManager* s_Instance;

    std::map<Components::FontType, Font*> m_Fonts;
};
