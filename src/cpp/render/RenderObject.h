#pragma once

#include <vector>

struct RenderBuffer;
struct Shader;
struct Texture;
struct Entity;
struct Element;

struct RenderObject
{
    ::RenderBuffer* RenderBuffer;
    ::Shader* Shader;
    ::Texture* Texture;
    std::vector<const ::Entity*> Entities;
};

struct GUIRenderObject
{
    ::RenderBuffer* RenderBuffer;
    ::Shader* Shader;
    ::Texture* Texture;
    ::Element* Element;
};
