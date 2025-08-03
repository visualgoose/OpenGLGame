#include "player.h"

#include "logging.h"

namespace OGLGAME::Entities
{
    TypeHierarchy Player::s_typeHierarchy = TypeHierarchy();

    void Player::S_CacheType()
    {
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