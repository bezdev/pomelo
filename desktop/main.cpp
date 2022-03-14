#include <memory>

#include "app.h"

int main()
{
    std::unique_ptr<App> app(App::GetInstance());
    app->SetStartScene(SCENE_CUBE);
    if (app->Initialize() != 0) return -1;
    app->Run();
}
