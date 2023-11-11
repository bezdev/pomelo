#pragma once

#include "engine/Components.h"
#include "engine/ECS.h"
#include "util/Util.h"

#ifdef BUILD_ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif
#ifdef BUILD_DESKTOP
#include <glad/glad.h>
#endif

#include <map>

class Texture
{
public:
    Texture(const char* filename);
    GLuint GetTextureID() { return m_TextureID; }
    int GetWidth() { return m_Width; }
    int GetHeight() { return m_Height; }
private:
    Texture();
    GLuint m_TextureID;
    int m_Width;
    int m_Height;
};

class TextureManager
{
public:
    static TextureManager* GetInstance()
    {
        if (!s_Instance) s_Instance = new TextureManager();
        return s_Instance;
    }

    static void DestroyInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    Texture* CreateTexture(const char* filename);

private:
    TextureManager() {};
    static TextureManager* s_Instance;

    std::map<const char*, Texture*> m_TextureMap;
};
