#include "util/Logger.h"

Logger::Logger(Level level, const char* name):
    m_Level(level),
    m_Name(name)
    {
    }

    const char *Logger::GetTag(Level level)
    {
        const char* LEVEL_STRINGS[] = {
            "[V] ",
            "[T] ",
            "[D] ",
            "[I] ",
            "[E] ",
        };

        return LEVEL_STRINGS[static_cast<int>(level)];
    }
