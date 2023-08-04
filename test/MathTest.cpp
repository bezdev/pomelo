#include <thread>

#include "glm/glm.hpp"

#include "Test.h"
#include "logger.h"

void PrintVector(const glm::vec3& v)
{
    LOGT("v: %f,%f,%f", v.x, v.y, v.z);
}

void PrintATAN(float x, float z)
{
    LOGT("atan2: %f", atan2(x, z));
    LOGT("atan2f: %f", atan2f(x, z));
}

void PrintEuler(float yaw, float pitch)
{
    glm::vec3 forward;
    forward.x = std::cos(pitch) * std::sin(yaw);
    forward.y = std::sin(pitch);
    forward.z = std::cos(pitch) * std::cos(yaw);
    forward = glm::normalize(forward);
    LOGT("PrintEuler: %f,%f,%f", forward.x, forward.y, forward.z);
}

TEST(VectorTest)
{
    glm::vec3 a(0, 1, 1);
    glm::vec3 b(0, 0, 0);

    glm::vec3 m_Target = glm::vec3(0,0,0);
    glm::vec3 m_Position = glm::vec3(0,0,10);

    auto direction = glm::normalize(m_Target - m_Position);
    // m_Position = glm::normalize(glm::cross(direction, glm::vec3(0.f, 1.f, 0.f)));
    PrintVector(direction);

    PrintATAN(1, 0);
    PrintATAN(0, 1);
    PrintATAN(-1, 0);
    PrintATAN(0, -1);

    PrintEuler(glm::radians(180.f), 0);
    PrintEuler(glm::radians(0.f), 0);
    PrintEuler(glm::radians(90.f), 0);
    PrintEuler(glm::radians(270.f), 0);
    // float yaw = atan2f(direction.x, direction.z);
    // float pitch = asin(direction.y);
    // // yaw += glm::radians(180.f);
    // glm::vec3 forward;
    // forward.x = std::cos(pitch) * std::sin(yaw);
    // forward.y = std::sin(pitch);
    // forward.z = std::cos(pitch) * std::cos(yaw);
    // // forward.x = std::cos(pitch) * std::sin(yaw);
    // // forward.y = std::sin(pitch);
    // // forward.z = std::cos(pitch) * std::cos(yaw);
    // // forward.x = cos(yaw) * cos(pitch);
    // // forward.y = sin(pitch);
    // // forward.z = cos(yaw) * cos(pitch);
    // direction = glm::normalize(forward);

    // LOGT("directon: %f,%f,%f", direction.x, direction.y, direction.z);
    // LOGT("position: %f,%f,%f", m_Position.x, m_Position.y, m_Position.z);
}
