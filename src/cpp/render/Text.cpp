#include "Text.h"

FontFiles Text::s_FONT_FILE_MAP[] = {
    { "assets/fonts/default.png", "assets/fonts/default.csv" },
};

Text::Text(EntityID entityID, char *text)
{
    m_Text = text;
    m_Font = FontManager::GetInstance()->CreateFont(s_FONT_FILE_MAP[Components::FontType::DEFAULT].Glyph);
    m_Texture = TextureManager::GetInstance()->CreateTexture(entityID, s_FONT_FILE_MAP[Components::FontType::DEFAULT].Atlas);
}
