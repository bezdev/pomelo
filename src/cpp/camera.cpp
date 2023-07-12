#include "camera.h"

#include "InputManager.h"

Camera* Camera::s_Instance = nullptr;

Camera::Camera():
    m_Height(0),
    m_Width(0),
    m_Ratio(0),
    m_NearPlane(3),
    m_FarPlane(500),
    m_CamTheta(0.f),
    m_CamPhi(0.f)
{
    InputManager::GetInstance()->RegisterCallback(InputEvent::MOUSE_MOVE, [&](InputEvent event, InputData data) {
        if (!InputManager::GetInstance()->IsKeyDown(InputEvent::MOUSE_BUTTON_LEFT)) return;

        m_CamTheta += (data.DX * MOVE_SCALE);
        m_CamPhi -= (data.DY * MOVE_SCALE);

        m_CamTheta = GET_ANGLE_IN_RANGE(m_CamTheta, 0, 2 * Constants::PI);
        m_CamPhi = CAP_ANGLE(m_CamPhi, MIN_PHI, MAX_PHI);

        m_Position = glm::vec3(
            m_LookAtDistance * std::cos(m_CamPhi) * std::sin(m_CamTheta),
            m_LookAtDistance * std::sin(m_CamPhi),
            m_LookAtDistance * std::cos(m_CamPhi) * std::cos(m_CamTheta)
        );

        // LOGD("CamTheta,CamPhi: %f,%f p: (%f,%f,%f)", TO_DEGRESS(m_CamTheta), TO_DEGRESS(m_CamPhi), m_Position.x, m_Position.y, m_Position.z );

        SetLookAt(m_Position, glm::vec3(0, 0, 0));
    });
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
