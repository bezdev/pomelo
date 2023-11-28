#pragma once

#include <map>
#include <vector>
#include <list>
#include <map>
#include <memory>

#ifdef BUILD_ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif
#ifdef BUILD_DESKTOP
#include <glad/glad.h>
#endif

#include "app/App.h"
#include "engine/ECS.h"
#include "engine/Font.h"
#include "engine/Mesh.h"
#include "engine/Scene.h"
#include "render/GUI.h"
#include "render/RenderBuffer.h"
#include "render/RenderObject.h"
#include "render/Shader.h"
#include "render/Text.h"
#include "render/Texture.h"
#include "util/Util.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


inline void CHECK_GL_ERROR(const char* label)
{
    for (GLint error = glGetError(); error; error = glGetError())
    {
        LOGE("%s: glError: (0x%x)\n", label, error);
    }
}

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
    void LoadGUI();
    void AddGUIElement(GUI::Element* element);

    void UpdateWindowSize(int width, int height);
    void Render();
    void RenderGUI();
    void UpdateFPS(const std::string& fps);
private:
    static Renderer* s_Instance;

    bool m_IsInitialized;
    bool m_IsDrawWireFrame;

    int m_ScreenWidth;
    int m_ScreenHeight;

    ShaderManager m_ShaderManager;
    GUI::GUI m_GUI;
    std::vector<RenderObject> m_RenderObjects;
    std::vector<GUIRenderObject> m_GUIRenderObjects;
    GUI::TextElement* m_FPSTextElement;

    void Cleanup();
};
