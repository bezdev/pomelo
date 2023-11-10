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

    TextureManager();
    void AddTexture(EntityID id, Components::Material* material);
    void CreateTexture(EntityID id, const char* filename);
    GLuint GetTextureID(EntityID id) { return m_TextureMap[id]; }

private:
    static TextureManager* s_Instance;

    std::map<EntityID, GLuint> m_TextureMap;
};
