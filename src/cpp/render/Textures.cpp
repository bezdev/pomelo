#include "Textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

TextureManager* TextureManager::s_Instance = nullptr;

TextureManager::TextureManager()
{
}

void TextureManager::AddTexture(EntityID id, Components::Material* material)
{
    // TODO: don't add same texture twice if material->Name already exists
    // TODO: call texture
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto fileData = Util::ReadFile(material->Name);
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
        LOGD("textureId: %d", textureId);
    }
    else
    {
        LOGE("Failed to load texture");
    }
    stbi_image_free(data);

    m_TextureMap[id] = textureId;
}

void TextureManager::CreateTexture(EntityID id, const char* filename)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
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
        LOGD("textureId: %d", textureId);
    }
    else
    {
        LOGE("Failed to load texture");
    }
    stbi_image_free(data);

    m_TextureMap[id] = textureId;
}
