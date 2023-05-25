#pragma once

#include "app.h"
#include "model.h"
#include "renderobjectcollection.h"
#include "scene.h"
#include "util.h"

// TODO: move to other file
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class App;

class Renderer {
public:
    static Renderer* GetInstance()
    {
        static Renderer* instance = new Renderer();
        //LOGD("Renderer::GetInstance()");
        return instance;
    };

    Renderer();
    ~Renderer();

    int Initialize();
    void LoadShaders();

    bool IsInitialized() { return m_IsInitialized; };
    void AddRenderObject(Model* model);
    void UpdateWindowSize(int width, int height);
    void Render();
private:
    bool m_IsInitialized;

    int m_ScreenWidth;
    int m_ScreenHeight;

    glm::mat4 m_ProjectionMatrix;

    //RenderObjectCollection m_RenderObjectCollection;
    std::vector<Model*> m_RenderObjects;

    static void CheckGlError(const char* op) {
        for (GLint error = glGetError(); error; error = glGetError()) {
            LOGI("after %s() glError (0x%x)\n", op, error);
        }
    }

    void Cleanup();
};
