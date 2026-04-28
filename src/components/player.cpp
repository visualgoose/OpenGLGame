#include "player.h"

#include "inputs.h"

#include <glm/geometric.hpp>

#include "client.h"
#include "glm/gtc/quaternion.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include <cmath>

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
        m_pTransform->m_rotation = glm::quat(glm::radians(glm::vec3(90.0f, 0.0f, 0.0f)));
    }

    void Player::Frame(const double deltaTime)
    {
        glm::vec2 mouseDelta = InputSystem::GetMouseDelta();
        mouseDelta *= static_cast<float>(deltaTime) * 200;
        mouseDelta = glm::radians(mouseDelta);

        m_camPitch -= mouseDelta.y;
        m_camPitch = glm::clamp(m_camPitch, -c_pitchLimit, c_pitchLimit);
        m_camYaw -= mouseDelta.x;
        m_camYaw = fmodf(m_camYaw, glm::radians(360.0f));

        m_pTransform->m_rotation = glm::quat(glm::vec3(m_camPitch, m_camYaw, 0.0f));

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