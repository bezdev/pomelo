#version 300 es
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 iColor;

out vec4 v_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
    v_color = iColor;
}
