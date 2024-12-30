#pragma once

#include "engine/ECS.h"
#include "util/Math.h"
#include "util/Util.h"

enum class CameraType
{
    FREE_LOOK,
    ORBIT,
    FOLLOW_TARGET
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
#ifdef USE_ENTT
    void SetTarget(entt::entity target);
#else
    void SetTarget(Entity* target);
#endif
    void SetYawPitch(float yaw, float pitch);
    void UpdateViewSize(int width, int height);
private:
    static Camera* s_Instance;

    CameraType m_CameraType;

#ifdef USE_ENTT
    entt::entity m_TargetEntity;
#else
    Entity* m_TargetEntity;
#endif

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