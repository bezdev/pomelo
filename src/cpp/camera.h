#pragma once

#include "glm/glm.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "util.h"

class Camera
{
public:
    static constexpr float MIN_PHI = -Constants::PI / 2 + .087;
    static constexpr float MAX_PHI = Constants::PI / 2 - .087;
    static constexpr float MOVE_SCALE = .0025f;

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

    glm::mat4 GetViewMatrix() { return m_ViewMatrix; }
    glm::mat4 GetProjectionMatrix() { return m_ProjectionMatrix; }

    void SetLookAt(glm::vec3 position, glm::vec3 target);
    void UpdateViewSize(int width, int height);
private:
    static Camera* s_Instance;

    float m_Height;
    float m_Width;
    float m_Ratio;
    float m_NearPlane;
    float m_FarPlane;
    float m_LookAtDistance;
    float m_CamTheta;
    float m_CamPhi;

    glm::vec3 m_Position;
    glm::vec3 m_Target;
    glm::vec3 m_LookAt;
    glm::vec3 m_PreviousTargetPosition;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;
};