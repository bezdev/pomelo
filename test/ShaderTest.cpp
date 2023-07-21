#include "Test.h"

#include "mesh.h"
#include "util.h"

TEST(ShaderParseOBJ)
{
    auto f = Util::ReadFile("assets/unit_axis.obj");

    Mesh::OBJData d;
    Mesh::ParseOBJ(f, d);

    LOGT("HI");
}