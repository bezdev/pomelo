#include <memory>

#include "app.h"

int main()
{
    std::unique_ptr<App> app(App::GetInstance());
    if (app->Initialize() != 0) return -1;
    app->Run();
}
