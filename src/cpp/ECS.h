// class Entity {
// public:
//     template <typename T>
//     void addComponent(unsigned int entity, std::shared_ptr<T> component) {
//         components[entity][T::getType()] = component;
//     }

//     template <typename T>
//     std::shared_ptr<T> getComponent(unsigned int entity) {
//         return std::static_pointer_cast<T>(components[entity][T::getType()]);
//     }

// private:
//     std::unordered_map<unsigned int, std::unordered_map<std::string, std::shared_ptr<Component>>> components;
// };


// #include <array>
// #include <bitset>
// #include <cstdint>
// #include <vector>

// constexpr std::size_t kMaxComponents = 32;
// constexpr std::size_t kMaxEntities = 10000;

// using ComponentType = std::size_t;
// using Entity = std::uint32_t;


// class Component {};

// class Ecs {
// public:
//   template <typename T>
//   ComponentType GetComponentType() {
//     static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
//     static ComponentType type = GetNextComponentType();
//     return type;
//   }

//   template <typename T>
//   void RegisterComponent() {
//     ComponentType type = GetComponentType<T>();
//     component_types_[type] = &typeid(T);
//     // components_[type].resize(kMaxEntities);
//   }

//   Entity CreateEntity() {
//     Entity entity = entities_.back();
//     entities_.pop_back();
//     return entity;
//   }

//   void DestroyEntity(Entity entity) {
//     entities_.push_back(entity);
//     for (auto& component_array : components_) {
//       component_array[entity] = nullptr;
//     }
//     entity_component_masks_[entity].reset();
//   }

//   template <typename T>
//   void AddComponent(Entity entity, T component) {
//     ComponentType type = GetComponentType<T>();
//     components_[type][entity] = std::make_unique<T>(std::move(component));
//     entity_component_masks_[entity][type] = true;
//   }

//   template <typename T>
//   void RemoveComponent(Entity entity) {
//     ComponentType type = GetComponentType<T>();
//     components_[type][entity] = nullptr;
//     entity_component_masks_[entity][type] = false;
//   }

//   template <typename T>
//   T& GetComponent(Entity entity) {
//     ComponentType type = GetComponentType<T>();
//     return *static_cast<T*>(components_[type][entity].get());
//   }

//   template <typename T>
//   bool HasComponent(Entity entity) {
//     ComponentType type = GetComponentType<T>();
//     return entity_component_masks_[entity][type];
//   }

//   std::bitset<kMaxComponents> GetComponentMask(Entity entity) {
//     return entity_component_masks_[entity];
//   }

//   template <typename... Ts>
//   std::vector<Entity> GetEntitiesWithComponents() {
//     std::vector<Entity> entities;
//     for (Entity entity = 0; entity < kMaxEntities; ++entity) {
//       bool has_components = true;
//       std::initializer_list<bool>{(has_components &= HasComponent<Ts>(entity))...};
//       if (has_components) {
//         entities.push_back(entity);
//       }
//     }
//     return entities;
//   }

// private:
//   ComponentType GetNextComponentType() {
//     static ComponentType next_type = 0;
//     return next_type++;
//   }

//   std::array<std::unique_ptr<Component>, kMaxEntities> components_[kMaxComponents];
//   std::array<std::bitset<kMaxComponents>, kMaxEntities> entity_component_masks_;
//   std::array<const std::type_info*, kMaxComponents> component_types_;
//   std::vector<Entity> entities_;
// };

// int main() {
//   Ecs ecs;

//   ecs.RegisterComponent<PositionComponent>();
//   ecs.RegisterComponent<VelocityComponent>();

//   Entity entity = ecs.CreateEntity();
//   ecs.AddComponent(entity, PositionComponent{1.0f, 2.0f});
//   ecs.AddComponent(entity, VelocityComponent{3.0f, 4.0f});

//   auto position = ecs.GetComponent<PositionComponent>(entity);
//   auto velocity = ecs.GetComponent<VelocityComponent>(entity);

//   std::vector<Entity> entities = ecs.GetEntitiesWithComponents<PositionComponent, VelocityComponent>();

//   return 0;
// }

#include <vector>
#include <unordered_map>
#include <bitset>
#include <memory>
#include <iostream>

template<typename T>
class Component;

using EntityID = std::size_t;
using ComponentTypeID = std::size_t;

constexpr std::size_t MAX_COMPONENTS = 32;

inline ComponentTypeID GetUniqueComponentTypeID() noexcept {
    static ComponentTypeID id = 0;
    return id++;
}

template<typename T>
std::vector<T>& GetComponentArray() noexcept {
    static std::vector<T> componentArray;
    return componentArray;
}

template<typename T>
ComponentTypeID GetComponentTypeID() noexcept {
    static ComponentTypeID typeID = GetUniqueComponentTypeID();
    return typeID;
}

class Entity {
public:
    Entity(EntityID id) : m_ID(id) {}

    EntityID getID() const noexcept {
        return m_ID;
    }

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        auto& componentArray = GetComponentArray<T>();
        if (componentArray.size() <= m_ID) {
            componentArray.resize(m_ID + 1);
        }
        T component(std::forward<Args>(args)...);
        componentArray[m_ID] = std::move(component);
        m_ComponentBitset[GetComponentTypeID<T>()] = true;
        return componentArray[m_ID];
    }

    template<typename T>
    T& GetComponent() {
        auto& componentArray = GetComponentArray<T>();
        return componentArray[m_ID];
    }

    template<typename T>
    bool HasComponent() const noexcept {
        return m_ComponentBitset[GetComponentTypeID<T>()];
    }

private:
    EntityID m_ID;
    std::bitset<MAX_COMPONENTS> m_ComponentBitset;
};

class ECS {
public:
    ECS() {}

    Entity& CreateEntity()
    {
        m_entities.push_back(m_nextEntityID++);
        return m_entities.back();
    }

private:
    std::vector<Entity> m_entities;
    EntityID m_nextEntityID = 0;
};
