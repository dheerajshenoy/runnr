#include "PlayerCollisionCallback.hpp"
#include "Components.hpp"
#include "GameScene.hpp"
#include "Entity.hpp"

PlayerCollisionCallback::PlayerCollisionCallback(GameScene *scene)
    : m_gameScene(scene)
{}

btScalar PlayerCollisionCallback::addSingleResult(btManifoldPoint& cp,
                                                  const btCollisionObjectWrapper* colObj0Wrap,
                                                  int partId0, int index0,
                                                  const btCollisionObjectWrapper* colObj1Wrap,
                                                  int partId1, int index1)
{
    const btCollisionObject* objA = colObj0Wrap->getCollisionObject();
    const btCollisionObject* objB = colObj1Wrap->getCollisionObject();

    btCollisionObject* other = (objA == m_gameScene->player->body) ? (btCollisionObject*)objB : (btCollisionObject*)objA;

    if (auto ptr = other->getUserPointer())
    {
        auto handle = static_cast<entt::entity>(reinterpret_cast<uintptr_t>(ptr));
        Entity entity { handle, m_gameScene };

        if (entity.HasComponent<PowerupComponent>())
        {
            auto &powerup = entity.GetComponent<PowerupComponent>();

            switch(powerup.Type)
            {
                case PowerupComponent::PowerupType::Jump:
                    m_gameScene->ApplyPowerupEffect(PowerupComponent::PowerupType::Jump);
                    m_gameScene->DestroyEntity(entity);
                    m_gameScene->dynamicsWorld->removeCollisionObject(other);
            }
        }
    }

    return 0;
}
