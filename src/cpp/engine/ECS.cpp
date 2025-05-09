#include "ECS.h"
#include "EventDispatcher.h"

ECS* ECS::s_Instance = nullptr;
entt::registry* ENTT::s_Instance = nullptr;

void PublishAddEntityEvent(ENTITY entity)
{
    LOGD("PublishAddEntityEvent: %d", entity);

    EntityData data { entity };
    Event e {
        EventType::ENTITY_CREATED,
        data
    };

    EventDispatcher::GetInstance()->Publish(e);
}
