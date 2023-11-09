#include "engine/Font.h"

FontManager* FontManager::s_Instance = nullptr;

Font::Font() {}

void Font::AddGlyph(char c, Glyph glyph)
{
    m_Glyphs[c] = glyph;
}

void Font::CreateFromFile(const char *filename)
{
    std::vector<char> data = Util::ReadFile(filename);

    std::string dataString(data.begin(), data.end());
    std::stringstream dataStream(std::move(dataString));
    std::string line;

    while (std::getline(dataStream, line)) {
        std::vector<std::string> split = Util::SplitString(line, std::string(","));

        m_Glyphs.emplace(
            static_cast<char>(std::stoi(split[0])),
            Glyph {
                std::stof(split[1]),
                std::stof(split[2]),
                std::stof(split[3]),
                std::stof(split[4]),
                std::stof(split[5]),
                std::stof(split[6]),
                std::stof(split[7]),
                std::stof(split[8]),
                std::stof(split[9]),
            }
        );
    }
}
