#pragma once

#include "entt/entt.hpp"
#include "Scene.hpp"
#include "Components.hpp"
#include "utils.hpp"

class Entity {

    public:
    Entity();
    Entity(const entt::entity &id,
           Scene *scene);
    ~Entity();

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args) noexcept
        {
            RUNNR_ASSERT(!HasComponent<T>(), "Entity already has this component");
            return m_currentScene->registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
        }

    template <typename T, typename... Args>
    T& AddOrReplaceComponent(Args&&... args) noexcept
        {
            T& component = m_currentScene->registry.emplace_or_replace<T>(m_entityHandle, std::forward<Args>(args)...);
            return component;
        }

    template <typename T>
    T& GetComponent() noexcept
        {
            RUNNR_ASSERT(HasComponent<T>(), "Entity does not have this component");
            return m_currentScene->registry.get<T>(m_entityHandle);
        }

    template <typename T>
    void RemoveComponent() noexcept
        {
            RUNNR_ASSERT(HasComponent<T>(), "Entity does not have this component");
            m_currentScene->registry.remove<T>(m_entityHandle);
        }

    template <typename T>
    bool HasComponent() const noexcept
        {
            return m_currentScene->registry.all_of<T>(m_entityHandle);
        }

    operator bool() const { return m_entityHandle != entt::null; }
    operator entt::entity() const { return m_entityHandle; }
    operator uint32_t() const { return (uint32_t) m_entityHandle; }

    entt::entity getHandle() const noexcept { return m_entityHandle; }

    bool operator==(const Entity &other) const {
        return m_entityHandle == other.m_entityHandle && m_currentScene == other.m_currentScene;
    }

    bool operator!=(const Entity &other) const {
        return !(*this == other);
    }

    private:
    entt::entity m_entityHandle { entt::null };
    Scene *m_currentScene { nullptr };
};
