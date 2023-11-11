#pragma once

#include "engine/Components.h"
#include "engine/ECS.h"
#include "engine/Font.h"
#include "render/Textures.h"
#include "util/Util.h"

#ifdef BUILD_ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif
#ifdef BUILD_DESKTOP
#include <glad/glad.h>
#endif

struct FontFiles
{
    const char* Atlas;
    const char* Glyph;
};

class Text
{
public:
    Text(EntityID entityID, char *text);
    char* GetText() { return m_Text; }
    Font* GetFont() { return m_Font; }
    Texture* GetTexture() { return m_Texture; }
private:
    static FontFiles s_FONT_FILE_MAP[];

    Text() {};
    char* m_Text;
    Font* m_Font;
    Texture* m_Texture;
};
