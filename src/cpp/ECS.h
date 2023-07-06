#pragma once

#include <vector>
#include <unordered_map>
#include <bitset>
#include <memory>
#include <iostream>

#include "components.h"

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
    Entity(EntityID id) : m_ID(id) {}

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
    T& GetComponent()
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

    // template<typename... T>
    // bool HasComponents() const noexcept {
    //     return (... && HasComponent<T>());
    // }

private:
    EntityID m_ID;
    std::bitset<MAX_COMPONENTS> m_ComponentBitset;
};

class ECS
{
public:
    ECS() {}

    std::vector<Entity> GetEntities() { return m_Entities; }

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
    std::vector<Entity> GetEntitiesWithComponents()
    {
        std::vector<Entity> entities;
        for (Entity entity : m_Entities)
        {
            if (entity.HasComponents<Ts...>())
            {
                entities.push_back(entity);
            }
        }
        return entities;
    }

private:
    std::vector<Entity> m_Entities;
    EntityID m_nextEntityID = 0;
};
