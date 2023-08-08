#include "Test.h"

#include "engine/Mesh.h"
#include "util/Util.h"

TEST(ShaderParseOBJ)
{
    auto f = Util::ReadFile("assets/unit_axis.obj");

    Mesh::OBJData d;
    Mesh::ParseOBJ(f, d);

    LOGT("HI");
}