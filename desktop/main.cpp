#include <memory>

#include "app/GLFWApp.h"

int main()
{
    std::unique_ptr<GLFWApp> app(GLFWApp::GetInstance());
    if (app->Initialize(SceneType::SCENE_SANDBOX) != 0) return -1;
    app->Run();
}
