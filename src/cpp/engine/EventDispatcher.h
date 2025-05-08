#pragma once

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

struct EntityPayload
{
    unsigned int entityId;
};

struct KeypressPayload
{
    char key;
};

using EventPayload = std::variant<std::monostate, EntityPayload, KeypressPayload>;

struct Event
{
    EventType type;
    EventPayload payload;
};

class EventDispatcher
{
public:
    using Callback = std::function<void(const Event &)>;

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
            const auto &callbacks = m_Subscribers[static_cast<size_t>(event.type)];
            for (const auto &callback : callbacks)
            {
                callback(event);
            }

            eventsToDispatch.pop();
        }
    }

private:
    std::array<std::vector<Callback>, static_cast<size_t>(EventType::NUM_EVENTS)> m_Subscribers;
    std::queue<Event> m_EventQueue;

    std::mutex m_SubscriberMutex;
    std::mutex m_EventQueueMutex;
};
