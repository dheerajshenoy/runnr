#include "Entity.hpp"

Entity::Entity()
    : m_entityHandle(entt::null), m_currentScene(nullptr)
{}

Entity::Entity(const entt::entity &id,
               Scene *scene)
    :  m_entityHandle(id), m_currentScene(scene)
{}

Entity::~Entity()
{}
