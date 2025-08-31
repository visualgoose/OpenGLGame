#include "player.h"

#include "inputs.h"

#include <glm/geometric.hpp>

#include "client.h"
#include "glm/gtc/quaternion.hpp"

namespace OGLGAME::Components
{
    bool Player::s_typeRegistered = false;
    TypeHierarchy Player::s_typeHierarchy = TypeHierarchy();

    void Player::RegisterType()
    {
        if (s_typeRegistered)
            return;
        s_typeHierarchy.AddChild("Player");
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

    void Player::Frame(double deltaTime)
    {
        InputSystem& inputSystem = Client::S_GetInstance().GetInputSystem();
        glm::vec2 mouseDelta = inputSystem.GetMouseDelta();
        glm::vec3 eulerRot = glm::eulerAngles(m_pTransform->m_rotation);
        eulerRot.x -= mouseDelta.y * static_cast<float>(deltaTime) * 5;
        eulerRot.x = glm::clamp(eulerRot.x, -89.0f, 89.0f);
        eulerRot.y -= mouseDelta.x * static_cast<float>(deltaTime) * 5;
        m_pTransform->m_rotation = glm::quat(eulerRot);

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
        g_log.Info("Player pos:")
            .NextLine("x: {}", m_pTransform->m_position.x)
            .NextLine("y: {}", m_pTransform->m_position.y)
            .NextLine("z: {}", m_pTransform->m_position.z);
    }
}