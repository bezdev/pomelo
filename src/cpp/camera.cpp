#include "camera.h"

Camera* Camera::s_Instance = nullptr;

Camera::Camera():
    m_Height(0),
    m_Width(0),
    m_Ratio(0),
    m_NearPlane(3),
    m_FarPlane(500)
{
}

void Camera::SetLookAt(glm::vec3 position, glm::vec3 target)
{
    m_Position = position;
    m_Target = target;
    m_LookAt = target - position;
    m_LookAtDistance = glm::length(m_LookAt);
    m_LookAt = glm::normalize(m_LookAt);
    m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, glm::vec3(0.f, 1.f, 0.f));
}

void Camera::UpdateViewSize(int width, int height)
{
    m_Width = width;
    m_Height = height;
    m_Ratio = 1.f * m_Width / m_Height;

    m_ProjectionMatrix = glm::frustum<float>(-m_Ratio, m_Ratio, -1.0f, 1.0f, m_NearPlane, m_FarPlane);
}
