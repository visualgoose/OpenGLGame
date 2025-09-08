#include "player.h"

#include "inputs.h"

#include <glm/geometric.hpp>

#include "client.h"
#include "glm/gtc/quaternion.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include "glm/gtx/string_cast.hpp"

namespace OGLGAME::Components
{
    bool Player::s_typeRegistered = false;
    TypeHierarchy Player::s_typeHierarchy = TypeHierarchy();

    void Player::RegisterType()
    {
        if (s_typeRegistered)
            return;
        s_typeHierarchy.AddChild("player");
        s_typeRegistered = true;
    }

    void Player::Setup()
    {
        GameObject* pGameObject = GetGameObject();
        m_pTransform = pGameObject->GetComponent<Transform>();
        if (!m_pTransform)
        {
            m_pTransform = pGameObject->AddComponent<Transform>();
        }
    }

    glm::quat QuaternionRotate(const glm::vec3& axis, float angle)
    {
        float angleRad = glm::radians(angle);
        auto axisNorm = glm::normalize(axis);

        float w = glm::cos(angleRad / 2);
        float v = glm::sin(angleRad / 2);
        glm::vec3 qv = axisNorm * v;

        return { w, qv };
    }

    void Player::Frame(double deltaTime)
    {
        glm::vec2 mouseDelta = InputSystem::GetMouseDelta();
        mouseDelta *= static_cast<float>(deltaTime) * 100;

        m_pTransform->m_rotation *= glm::quat(glm::eulerAngleXY(mouseDelta.x, mouseDelta.y));


        m_movement = { 0.0f, 0.0f };
        if (g_pForward->m_state)
            m_movement.y += 1.0f;
        if (g_pBackward->m_state)
            m_movement.y -= 1.0f;
        if (g_pRight->m_state)
            m_movement.x += 1.0f;
        if (g_pLeft->m_state)
            m_movement.x -= 1.0f;

        if (m_movement.y > 0.0f && m_movement.x > 0.0f)
            m_movement = glm::normalize(m_movement);

        m_movement *= static_cast<float>(deltaTime);

        m_pTransform->m_position += 10.0f * (m_movement.x * (m_pTransform->m_rotation * glm::vec3(1.0f, 0.0f, 0.0f)) +
            m_movement.y * (m_pTransform->m_rotation * glm::vec3(0.0f, 0.0f, -1.0f)));
    }
}