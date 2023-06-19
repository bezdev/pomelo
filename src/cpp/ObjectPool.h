#pragma once

#include <memory>
#include <array>
#include <list>

#include "logger.h"

template <typename T, std::size_t N>
class ObjectPool
{
public:
    ObjectPool()
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            m_FreeList.push_back(&m_Pool[i]);
        }
    };

    template <typename... Args>
    T& Create(Args&&... args)
    {
        if (m_FreeList.empty())
        {
            const char* error = "ObjectPool is full";
            LOGE(error);
            throw error;
        }

        T& object = *m_FreeList.back();
        m_FreeList.pop_back();
        object.~T();
        new (&object) T(std::forward<Args>(args)...);
        m_ObjectCount++;
        return object;
    }

    void Destroy(T& object)
    {
        object.~T();
        m_FreeList.push_back(&object);
        m_ObjectCount--;
    }

    std::size_t Count()
    {
        return m_ObjectCount;
    }

private:
  std::size_t m_ObjectCount = 0;
  std::array<T, N> m_Pool;
  std::list<T*> m_FreeList;
};
