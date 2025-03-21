#pragma once

#include "bullet.hpp"

class GameScene;

class PlayerCollisionCallback : public btCollisionWorld::ContactResultCallback
{
    public:
    PlayerCollisionCallback(GameScene *gameScene);

    btScalar addSingleResult(btManifoldPoint& cp,
                             const btCollisionObjectWrapper* colObj0Wrap,
                             int partId0, int index0,
                             const btCollisionObjectWrapper* colObj1Wrap,
                             int partId1, int index1) override;

    private:
    GameScene *m_gameScene { nullptr };

};
