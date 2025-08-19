#include "player.h"

#include "logging.h"
#include "interface/renderable.h"

namespace OGLGAME::Entities
{
    bool Player::s_typeHierarchyInitialized = false;
    TypeHierarchy Player::s_typeHierarchy = TypeHierarchy();

    void Player::S_CacheType()
    {
        if (s_typeHierarchyInitialized)
            return;
        s_typeHierarchy.AddChild("player");
    }

    void Player::Tick(double deltaTime)
    {

    }
    void Player::Frame(double deltaTime)
    {
        g_log.Info("Player::Frame({})", deltaTime)
            .NextLine("IsPlayer: {}", GetTypeHierarchy().Is(Player::S_GetTypeHierarchy()));
    }
}
