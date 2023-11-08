#include <thread>

#include "Test.h"

#include "util/Logger.h"
#include "engine/Font.h"

TEST(CreateFontFromFileTest)
{
    Font f;
    f.CreateFromFile("assets/fonts/default.csv");
}
