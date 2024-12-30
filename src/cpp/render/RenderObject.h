#pragma once

#include <vector>

struct RenderBuffer;
class Shader;
class Texture;
class Entity;

namespace GUI
{
    class Element;
}

struct RenderObject
{
    ::RenderBuffer* RenderBuffer;
    ::Shader* Shader;
    ::Texture* Texture;
    std::vector<ENTITY> Entities;
};

struct GUIRenderObject
{
    ::RenderBuffer* RenderBuffer;
    ::Shader* Shader;
    ::Texture* Texture;
    GUI::Element* Element;
};
