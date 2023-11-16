#pragma once

#include <string>

#include "engine/Components.h"
#include "engine/ECS.h"
#include "engine/Font.h"
#include "render/Texture.h"
#include "render/RenderBuffer.h"
#include "util/Util.h"

class RenderBuffer;

struct FontFiles
{
    const char* Atlas;
    const char* Glyph;
};

class Text
{
public:
    Text(std::string text);
    const std::string& GetText() { return m_Text; }
    Font* GetFont() { return m_Font; }
    Texture* GetTexture() { return m_Texture; }
    RenderBuffer* GetRenderBuffer() { return m_RenderBuffer; }
private:
    static FontFiles s_FONT_FILE_MAP[];

    Text() {};
    std::string m_Text;
    Font* m_Font;
    Texture* m_Texture;
    RenderBuffer* m_RenderBuffer;
};

class TextManager
{
public:
    static TextManager* GetInstance()
    {
        if (!s_Instance) s_Instance = new TextManager();
        return s_Instance;
    }

    static void DestroyInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    TextManager();
    int AddText(std::string text);
    Text* GetText(int id) { return m_Texts[id]; }

private:
    static TextManager* s_Instance;
    std::vector<Text*> m_Texts;
};
