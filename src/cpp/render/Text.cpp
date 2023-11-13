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
}

TextManager::TextManager()
{
}

int TextManager::AddText(std::string text)
{
    m_Texts.push_back(new Text(text));
    return m_Texts.size() - 1;
}
