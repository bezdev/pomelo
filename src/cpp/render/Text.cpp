#include "Text.h"

TextManager* TextManager::s_Instance = nullptr;

FontFiles Text::s_FONT_FILE_MAP[] = {
    { "assets/fonts/default.png", "assets/fonts/default.csv" },
};

Text::Text(std::string text)
{
    m_Text = std::move(text);
    m_Font = FontManager::GetInstance()->CreateFont(s_FONT_FILE_MAP[Components::FontType::DEFAULT].Glyph);
    m_Texture = TextureManager::GetInstance()->CreateTexture(s_FONT_FILE_MAP[Components::FontType::DEFAULT].Atlas);

    std::vector<VEC3> vertices;
    std::vector<VEC2> texCoords;
    std::vector<unsigned short> indices;
    CreateData(vertices, texCoords, indices);
    m_RenderBuffer = RenderBufferManager::GetInstance()->CreateText(vertices, texCoords, indices);
}

void Text::UpdateText(std::string text)
{
    m_Text = text;
    std::vector<VEC3> vertices;
    std::vector<VEC2> texCoords;
    std::vector<unsigned short> indices;
    CreateData(vertices, texCoords, indices);
    RenderBufferManager::UpdateText(m_RenderBuffer, vertices, texCoords, indices);
}

void Text::CreateData(std::vector<VEC3>& vertices, std::vector<VEC2>& texCoords, std::vector<unsigned short>& indices)
{
    float scale = 1.f / m_Font->GetMaxHeight();
    float advance = 0.f;
    int i = 0;
    for (auto it = m_Text.begin(); it != m_Text.end(); ++it)
    {
        Glyph g = m_Font->GetGlyph(*it);

        float x0 = (g.BitmapLeft + advance) * scale;
        float y0 = g.BitmapTop * scale;
        float x1 = (g.Width + advance) * scale;
        float y1 = g.Height * scale;

        g.u1 /= m_Texture->GetWidth();
        g.u2 /= m_Texture->GetWidth();
        g.v1 /= m_Texture->GetHeight();
        g.v2 /= m_Texture->GetHeight();

        g.v1 = 1.f - g.v1;
        g.v2 = 1.f - g.v2;

        vertices.push_back(VEC3(x0, y0, 0.0f));
        vertices.push_back(VEC3(x1, y0, 0.0f));
        vertices.push_back(VEC3(x0, y1, 0.0f));
        vertices.push_back(VEC3(x1, y1, 0.0f));

        texCoords.push_back(VEC2(g.u1, g.v1));
        texCoords.push_back(VEC2(g.u2, g.v1));
        texCoords.push_back(VEC2(g.u1, g.v2));
        texCoords.push_back(VEC2(g.u2, g.v2));

        indices.push_back(0 + i * 4);
        indices.push_back(1 + i * 4);
        indices.push_back(2 + i * 4);
        indices.push_back(2 + i * 4);
        indices.push_back(1 + i * 4);
        indices.push_back(3 + i * 4);

        advance += g.Advance;
        i++;
    }
}

TextManager::TextManager()
{
}

int TextManager::AddText(std::string text)
{
    m_Texts.push_back(new Text(text));
    return m_Texts.size() - 1;
}
