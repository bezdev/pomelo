#pragma once

#include <vector>

struct RenderBuffer;
struct Shader;
struct Texture;
struct Entity;

struct RenderObject
{
    ::RenderBuffer* RenderBuffer;
    ::Shader* Shader;
    ::Texture* Texture;
    std::vector<const ::Entity*> Entities;
};
