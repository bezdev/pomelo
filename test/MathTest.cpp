#include <thread>

#include "glm/glm.hpp"

#include "Test.h"
#include "logger.h"

void PrintVector(const glm::vec3& v)
{
    LOGT("v: %f,%f,%f", v.x, v.y, v.z);
}

TEST(VectorTest)
{
    glm::vec3 a(1, 1, 1);
    glm::vec3 b(0, 0, 0);

    PrintVector(b-a);
}
