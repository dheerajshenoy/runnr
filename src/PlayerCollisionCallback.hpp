#pragma once

#include "bullet.hpp"
class Player;

class PlayerCollisionCallback : public btCollisionWorld::ContactResultCallback
{
    public:
    PlayerCollisionCallback(Player *player);

    btScalar addSingleResult(btManifoldPoint& cp,
                             const btCollisionObjectWrapper* colObj0Wrap,
                             int partId0, int index0,
                             const btCollisionObjectWrapper* colObj1Wrap,
                             int partId1, int index1) override;

    private:
    Player *m_player { nullptr };

};
