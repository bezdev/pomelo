#pragma once

#include "ECS.h"

#include <array>
#include <functional>
#include <mutex>
#include <queue>
#include <variant>
#include <vector>

enum class EventType : unsigned int
{
    ENTITY_CREATED = 0,
    ENTITY_DELETED,
    KEYPRESS_P,
    KEYPRESS_R,
    NUM_EVENTS,
};

struct EntityData
{
    ENTITY Entity;
};

struct KeypressData
{
    char Key;
};

using EventData = std::variant<std::monostate, EntityData, KeypressData>;

struct Event
{
    EventType Type;
    EventData Data;
};

class EventDispatcher
{
public:
    using Callback = std::function<void(const Event &)>;

    static EventDispatcher* GetInstance()
    {
        if (!s_Instance) s_Instance = new EventDispatcher();
        return s_Instance;
    }

    static void DestoryInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    EventDispatcher()
    {
        m_Subscribers.fill({});
    }

    void Subscribe(EventType type, Callback callback)
    {
        std::lock_guard<std::mutex> lock(m_SubscriberMutex);
        m_Subscribers[static_cast<size_t>(type)].emplace_back(std::move(callback));
    }

    void Publish(const Event &event)
    {
        std::lock_guard<std::mutex> lock(m_EventQueueMutex);
        m_EventQueue.push(event);
    }

    void Dispatch()
    {
        std::queue<Event> eventsToDispatch;

        {
            std::lock_guard<std::mutex> lock(m_EventQueueMutex);
            std::swap(m_EventQueue, eventsToDispatch);
        }

        while (!eventsToDispatch.empty())
        {
            const Event &event = eventsToDispatch.front();

            std::lock_guard<std::mutex> lock(m_SubscriberMutex);
            const auto &callbacks = m_Subscribers[static_cast<size_t>(event.Type)];
            for (const auto &callback : callbacks)
            {
                callback(event);
            }

            eventsToDispatch.pop();
        }
    }

private:
    static EventDispatcher *s_Instance;

    std::array<std::vector<Callback>, static_cast<size_t>(EventType::NUM_EVENTS)> m_Subscribers;
    std::queue<Event> m_EventQueue;

    std::mutex m_SubscriberMutex;
    std::mutex m_EventQueueMutex;
};
