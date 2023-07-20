#pragma once

#include <map>
#include <vector>
#include <unordered_map>

#ifdef BUILD_ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif
#ifdef BUILD_DESKTOP
#include <glad/glad.h>
#endif


#include "app.h"
#include "ECS.h"
#include "shader.h"
#include "mesh.h"
#include "buffers.h"
#include "scene.h"
#include "util.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct RenderObject
{
    ::RenderBuffer* RenderBuffer;
    ::Shader* Shader;
    ::Entity* Entity;
    Components::Material* Material;
    Components::Mesh* Mesh;
    Components::Transform* Transform;
};

class Renderer
{
public:
    static Renderer* GetInstance()
    {
        if (!s_Instance) s_Instance = new Renderer();
        return s_Instance;
    }

    static void DestroyInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    Renderer();
    ~Renderer();

    int Initialize();

    bool IsInitialized() { return m_IsInitialized; };
    void LoadEntities(const std::vector<Entity>& entities);
    void UpdateWindowSize(int width, int height);
    void Render();
private:
    static Renderer* s_Instance;

    bool m_IsInitialized;
    bool m_IsDrawWireFrame;

    int m_ScreenWidth;
    int m_ScreenHeight;

    ShaderManager m_ShaderManager;
    RenderBufferManager m_RenderBufferManager;

    std::vector<RenderObject> m_RenderQueue;
    std::unordered_map<int, RenderBuffer*> m_RenderBuffers;

    static void CheckGLError(const char* op)
    {
        for (GLint error = glGetError(); error; error = glGetError())
        {
            LOGI("after %s() glError (0x%x)\n", op, error);
        }
    }

    void Cleanup();
};
