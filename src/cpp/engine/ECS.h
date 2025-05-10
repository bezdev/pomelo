#pragma once

#include <algorithm>
#include <vector>
#include <bitset>
#include <memory>
#include <iostream>

#include "engine/Components.h"
#include "vendor/entt/entt.hpp"

#define USE_ENTT

using EntityID = std::size_t;
using ComponentTypeID = std::size_t;

constexpr std::size_t MAX_COMPONENTS = 32;

inline ComponentTypeID GetUniqueComponentTypeID() noexcept
{
    static ComponentTypeID id = 0;
    return id++;
}

template<typename T>
std::vector<T>& GetComponentArray() noexcept
{
    static std::vector<T> componentArray;
    return componentArray;
}

template<typename T>
ComponentTypeID GetComponentTypeID() noexcept
{
    static ComponentTypeID typeID = GetUniqueComponentTypeID();
    return typeID;
}

class Entity
{
public:
    Entity(EntityID id): m_ID(id) {}
    Entity(const Entity& other):
        m_ID(other.m_ID),
        m_ComponentBitset(other.m_ComponentBitset)
    {}

    EntityID GetID() const noexcept
    {
        return m_ID;
    }

    template<typename T, typename... Args>
    Entity& AddComponent(Args&&... args)
    {
        auto& componentArray = GetComponentArray<T>();
        if (componentArray.size() <= m_ID)
        {
            componentArray.resize(m_ID + 1);
        }
        T component(std::forward<Args>(args)...);
        componentArray[m_ID] = std::move(component);
        m_ComponentBitset[GetComponentTypeID<T>()] = true;
        return *this;
    }

    template<typename T>
    void RemoveComponent()
    {
        auto& componentArray = GetComponentArray<T>();
        componentArray[m_ID] = T();
        m_ComponentBitset[GetComponentTypeID<T>()] = false;
    }

    template<typename T>
    T& GetComponent() const noexcept
    {
        auto& componentArray = GetComponentArray<T>();
        return componentArray[m_ID];
    }

    template<typename T>
    bool HasComponent() const noexcept
    {
        return m_ComponentBitset[GetComponentTypeID<T>()];
    }

    template<typename T>
    bool HasComponents() const noexcept
    {
        return HasComponent<T>();
    }

    template<typename T, typename T2, typename... Rest>
    bool HasComponents() const noexcept
    {
        return HasComponent<T>() && HasComponents<T2, Rest...>();
    }

private:
    EntityID m_ID;
    std::bitset<MAX_COMPONENTS> m_ComponentBitset;
};

class ECS
{
public:
    static ECS* GetInstance()
    {
        if (!s_Instance) s_Instance = new ECS();
        return s_Instance;
    }

    static void DestoryInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    ECS() {}

    std::vector<Entity>& GetEntities() { return m_Entities; }

    Entity& CreateEntity()
    {
        m_Entities.push_back(m_nextEntityID++);
        return m_Entities.back();
    }

    void RemoveEntity(EntityID id)
    {

        auto it = std::find_if(m_Entities.begin(), m_Entities.end(), [id](const Entity& e)
        {
            return e.GetID() == id;
        });

        if (it != m_Entities.end())
        {
            m_Entities.erase(it);
        }
    }

    template <typename... Ts>
    std::vector<Entity*> GetEntitiesWithComponents()
    {
        std::vector<Entity*> entities;
        for (Entity& entity : m_Entities)
        {
            if (entity.HasComponents<Ts...>())
            {
                entities.push_back(&entity);
            }
        }
        return entities;
    }

private:
    static ECS* s_Instance;
    std::vector<Entity> m_Entities;
    EntityID m_nextEntityID = 0;
};

class ENTT
{
public:
    static entt::registry* GetInstance()
    {
        if (!s_Instance) s_Instance = new entt::registry();
        return s_Instance;
    }

    static void DestoryInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }
private:
    static entt::registry* s_Instance;
};


#ifdef USE_ENTT
#define ENTITY entt::entity
#else
#define ENTITY Entity*
#endif

class Event;
void PublishCreateEntityEvent(ENTITY entity);

template<typename ComponentType, typename... Args>
ComponentType& AddComponent(ENTITY entity, Args&&... args)
{
#ifdef USE_ENTT
    auto& result = ENTT::GetInstance()->emplace<ComponentType>(entity, std::forward<Args>(args)...);
#else
    auto& result = entity->AddComponent<ComponentType>(std::forward<Args>(args)...);
#endif

    return result;
}

inline ENTITY CreateEntity()
{
    ENTITY e;
#ifdef USE_ENTT
    e = ENTT::GetInstance()->create();
#else
    e = (&ECS::GetInstance()->CreateEntity());
#endif

    PublishCreateEntityEvent(e);

    return e;
}

#define CREATE_ENTITY() CreateEntity()
#define ADD_COMPONENT(entity, component, ...) AddComponent<component>(entity, __VA_ARGS__)

#ifdef USE_ENTT
#define GET_ENTITIES_WITH_COMPONENTS(...) ENTT::GetInstance()->view<__VA_ARGS__>()
#define GET_COMPONENT(entity, component) ENTT::GetInstance()->get<component>(entity)
#define HAS_COMPONENT(entity, component) ENTT::GetInstance()->all_of<component>(entity)
#else
#define GET_ENTITIES_WITH_COMPONENTS(...) ECS::GetInstance()->GetEntitiesWithComponents<__VA_ARGS__>()
#define GET_COMPONENT(entity, component) entity->GetComponent<component>()
#define HAS_COMPONENT(entity, component) entity->HasComponent<component>()
#endif
