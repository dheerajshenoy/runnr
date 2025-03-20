#include "PlayerCollisionCallback.hpp"
#include "Player.hpp"

PlayerCollisionCallback::PlayerCollisionCallback(Player *player)
    : m_player(player)
{}

btScalar PlayerCollisionCallback::addSingleResult(btManifoldPoint& cp,
                                                  const btCollisionObjectWrapper* colObj0Wrap,
                                                  int partId0, int index0,
                                                  const btCollisionObjectWrapper* colObj1Wrap,
                                                  int partId1, int index1)
{
    const btCollisionObject* objA = colObj0Wrap->getCollisionObject();
    const btCollisionObject* objB = colObj1Wrap->getCollisionObject();

    btCollisionObject* other = (objA == m_player->body) ? (btCollisionObject*)objB : (btCollisionObject*)objA;

    if (auto ptr = other->getUserPointer())
    {
    }

    return 0;
}
