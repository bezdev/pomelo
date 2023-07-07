#include <memory>

#include "GLFWApp.h"

int main()
{
    std::unique_ptr<GLFWApp> app(GLFWApp::GetInstance());
    if (app->Initialize(SCENE_CUBE) != 0) return -1;
    app->Run();
}
