#pragma once

#include "util/Math.h"
#include "util/Util.h"

enum class CameraType
{
    FREE_LOOK,
    ORBIT
};

class Camera
{
public:
    static constexpr float MIN_PHI = -Constants::PI / 2 + .087;
    static constexpr float MAX_PHI = Constants::PI / 2 - .087;
    static constexpr float MOVE_SCALE = .0025f;
    static constexpr float CAMERA_SPEED = 50.f;

    static Camera* GetInstance()
    {
        if (!s_Instance) s_Instance = new Camera();
        return s_Instance;
    }

    static void DestroyInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    Camera();

    void Update(float delta);

    glm::mat4 GetViewMatrix() { return m_ViewMatrix; }
    glm::mat4 GetProjectionMatrix() { return m_ProjectionMatrix; }

    void SetCameraType(CameraType type) { m_CameraType = type; }
    void SetLookAt(const VEC3& position, const VEC3& target);
    void SetYawPitch(float yaw, float pitch);
    void UpdateViewSize(int width, int height);
private:
    static Camera* s_Instance;

    CameraType m_CameraType;
    float m_ScreenX;
    float m_ScreenY;
    float m_Height;
    float m_Width;
    float m_Ratio;
    float m_NearPlane;
    float m_FarPlane;
    float m_LookAtDistance;
    float m_Pitch;
    float m_Yaw;

    glm::vec3 m_Position;
    glm::vec3 m_Target;
    glm::vec3 m_LookAt;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;
};