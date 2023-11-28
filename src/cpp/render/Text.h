#pragma once

#include <string>

#include "engine/Components.h"
#include "engine/ECS.h"
#include "engine/Font.h"
#include "render/Texture.h"
#include "render/RenderBuffer.h"
#include "util/Util.h"

using TextID = std::size_t;

struct FontFiles
{
    const char* Atlas;
    const char* Glyph;
};

class Text
{
public:
    Text(TextID id, std::string text);

    void UpdateText(std::string text);

    TextID GetTextID() { return m_TextID; }
    const std::string& GetText() { return m_Text; }
    float GetWidth() { return m_Width; }
    Font* GetFont() { return m_Font; }
    Texture* GetTexture() { return m_Texture; }
    RenderBuffer* GetRenderBuffer() { return m_RenderBuffer; }
private:
    static FontFiles s_FONT_FILE_MAP[];

    Text() {};
    void CreateData(std::vector<VEC3>& vertices, std::vector<VEC2>& texCoords, std::vector<unsigned short>& indices);
    void SetText(std::string text);

    TextID m_TextID;
    std::string m_Text;
    float m_Width;
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
    Text* CreateText(std::string text);
    Text* GetText(TextID id) { return m_Texts[id]; }

private:
    static TextManager* s_Instance;
    std::vector<Text*> m_Texts;
};
