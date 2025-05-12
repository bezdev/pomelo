#include "engine/Camera.h"
#include "Camera.h"
#include "engine/EventDispatcher.h"
#include "engine/InputManager.h"

Camera *Camera::s_Instance = nullptr;

Camera::Camera()
    : m_CameraType(CameraType::ORBIT),
      // m_TargetEntity(nullptr),
      m_Height(0), m_Width(0), m_Ratio(0), m_NearPlane(3), m_FarPlane(500), m_ScreenX(-1.f), m_ScreenY(-1.f)
{
    EventDispatcher::GetInstance()->Subscribe(EventType::INPUT_EVENT, [this](const Event &e) {
        if (std::holds_alternative<InputEventData>(e.Data))
        {
            const auto &data = std::get<InputEventData>(e.Data);

            if (data.Event == InputEvent::MOUSE_MOVE && m_CameraType == CameraType::ORBIT)
            {
                if (!InputManager::GetInstance()->IsKeyDown(InputEvent::MOUSE_BUTTON_LEFT))
                    return;

                float camTheta = atan2f(m_LookAt.x, m_LookAt.z);
                float camPhi = asinf(m_LookAt.y);

                camTheta -= (data.Data.DX * MOVE_SCALE);
                camPhi -= (data.Data.DY * MOVE_SCALE);

                camTheta = GET_ANGLE_IN_RANGE(camTheta, 0, 2 * Constants::PI);
                camPhi = CAP_ANGLE(camPhi, MIN_PHI, MAX_PHI);

                m_Position = m_Target - glm::vec3(m_LookAtDistance * std::cos(camPhi) * std::sin(camTheta),
                                                  m_LookAtDistance * std::sin(camPhi),
                                                  m_LookAtDistance * std::cos(camPhi) * std::cos(camTheta));

                SetLookAt(m_Position, m_Target);
            }
        }
    });

    EventDispatcher::GetInstance()->Subscribe(EventType::INPUT_EVENT, [this](const Event &e) {
        if (std::holds_alternative<InputEventData>(e.Data))
        {
            const auto &data = std::get<InputEventData>(e.Data);

            if (m_CameraType == CameraType::FREE_LOOK && data.Event == InputEvent::MOUSE_BUTTON_LEFT &&
                data.Data.Action == InputAction::DOWN)
            {
                m_ScreenX = m_ScreenY = -1;
            }
        }
    });
}

void Camera::Update(float delta)
{
    if (m_CameraType == CameraType::FREE_LOOK)
    {
        float mouseSensitivity = .25f;
        float velocity = CAMERA_SPEED / 1000 * delta;

        glm::vec3 direction = glm::normalize(m_Target - m_Position);

        if (InputManager::GetInstance()->IsKeyDown(InputEvent::KEY_W))
            m_Position += direction * velocity;
        if (InputManager::GetInstance()->IsKeyDown(InputEvent::KEY_S))
            m_Position -= direction * velocity;
        if (InputManager::GetInstance()->IsKeyDown(InputEvent::KEY_A))
            m_Position -= glm::normalize(glm::cross(direction, glm::vec3(0.f, 1.f, 0.f))) * velocity;
        if (InputManager::GetInstance()->IsKeyDown(InputEvent::KEY_D))
            m_Position += glm::normalize(glm::cross(direction, glm::vec3(0.f, 1.f, 0.f))) * velocity;

        float yaw = m_Yaw;
        float pitch = m_Pitch;

        if (InputManager::GetInstance()->IsKeyDown(InputEvent::MOUSE_BUTTON_LEFT))
        {
            InputData data = InputManager::GetInstance()->GetData(InputEvent::MOUSE_MOVE);

            float xOffset = data.X - m_ScreenX;
            float yOffset = data.Y - m_ScreenY;
            if (m_ScreenX < 0 && m_ScreenY < 0)
                xOffset = yOffset = 0;

            m_ScreenX = data.X;
            m_ScreenY = data.Y;

            yaw -= glm::radians(mouseSensitivity * xOffset);
            yaw = fmod(yaw, 2.0f * Constants::PI);
            if (yaw < 0.0f)
                yaw += 2.0f * Constants::PI;

            pitch -= glm::radians(mouseSensitivity * yOffset);
        }

        SetYawPitch(yaw, pitch);
    }
    else if (m_CameraType == CameraType::FOLLOW_TARGET)
    {
    }
}

void Camera::SetLookAt(const VEC3 &position, const VEC3 &target)
{
    m_Position = position;
    m_Target = target;
    m_LookAt = target - position;
    m_LookAtDistance = glm::length(m_LookAt);
    m_LookAt = glm::normalize(m_LookAt);

    m_Yaw = atan2(m_LookAt.x, m_LookAt.z);
    m_Pitch = asin(m_LookAt.y);

    // LOGD("position: x,y,z: %f,%f,%f", position.x, position.y, position.z);
    // LOGD("target: x,y,z: %f,%f,%f", target.x, target.y, target.z);
    // LOGD("m_LookAt: x,y,z: %f,%f,%f", m_LookAt.x, m_LookAt.y, m_LookAt.z);
    m_ViewMatrix = glm::lookAt(m_Position, m_Target, V_UP);
}

#ifdef USE_ENTT
void Camera::SetTarget(entt::entity target)
{
    m_TargetEntity = target;
}
#else
void Camera::SetTarget(Entity *target)
{
    m_TargetEntity = target;
}
#endif

void Camera::SetYawPitch(float yaw, float pitch)
{
    glm::vec3 direction;
    direction.x = std::cos(pitch) * std::sin(yaw);
    direction.y = std::sin(pitch);
    direction.z = std::cos(pitch) * std::cos(yaw);
    direction = glm::normalize(direction);

    SetLookAt(m_Position, m_Position + direction);

    m_Yaw = yaw;
    m_Pitch = pitch;
}

void Camera::UpdateViewSize(int width, int height)
{
    m_Width = width;
    m_Height = height;
    m_Ratio = 1.f * m_Width / m_Height;

    m_ProjectionMatrix = glm::frustum<float>(-m_Ratio, m_Ratio, -1.0f, 1.0f, m_NearPlane, m_FarPlane);
}