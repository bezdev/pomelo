#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

TextureManager* TextureManager::s_Instance = nullptr;

Texture::Texture(const char* filename)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto fileData = Util::ReadFile(filename);
    int width, height, channelsCount;
    int channels = 0;
    unsigned char* data = stbi_load_from_memory(
        reinterpret_cast<const unsigned char*>(fileData.data()),
        static_cast<int>(fileData.size()),
        &width,
        &height,
        &channelsCount,
        channels
    );

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        LOGD("textureID: %d", textureID);
    }
    else
    {
        LOGE("Failed to load texture");
    }
    stbi_image_free(data);

    m_TextureID = textureID;
    m_Width = width;
    m_Height = height;
}

Texture* TextureManager::CreateTexture(const char* filename)
{
    if (m_TextureMap.find(filename) != m_TextureMap.end()) return m_TextureMap[filename];

    Texture* texture = new Texture(filename);
    m_TextureMap[filename] = texture;

    return texture;
}

