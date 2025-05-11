#include "InputManager.h"

#include "engine/EventDispatcher.h"

InputManager::InputManager()
    : m_X(0.f), m_Y(0.f), m_DX(0.f), m_DY(0.f), m_KeyDownVector(static_cast<size_t>(InputEvent::COUNT)),
      m_InputData(static_cast<size_t>(InputEvent::COUNT))
{
}

void InputManager::RegisterCallback(InputEvent event, InputCallback callback)
{
    m_Callbacks[event].push_back(callback);
}

void InputManager::OnEvent(InputEvent event, InputData data)
{
    if (data.Action == InputAction::DOWN)
        m_KeyDownVector[static_cast<size_t>(event)] = true;
    else if (data.Action == InputAction::UP)
        m_KeyDownVector[static_cast<size_t>(event)] = false;

    if (data.X >= 0 && data.Y >= 0)
    {
        auto PX = m_X;
        auto PY = m_Y;

        m_X = data.X;
        m_Y = data.Y;
        m_DX = data.DX = m_X - PX;
        m_DY = data.DY = m_Y - PY;
    }

    m_InputData[static_cast<size_t>(event)] = data;
    // if (data.Action != InputAction::MOVE) LOGD("InputManager::OnEvent: event: %d, data: { %d, %f, %f, %f, %f }",
    // static_cast<int>(event), data.Action, data.X, data.Y, data.DX, data.DY);

    for (auto &callback : m_Callbacks[event])
    {
        callback(event, data);
    }
    EventDispatcher::GetInstance()->Publish(Event{EventType::INPUT_EVENT, InputEventData{event, data}}, true);
}
