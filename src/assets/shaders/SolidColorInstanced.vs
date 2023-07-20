#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vInstancePosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    mat4 model = modelMatrix;
    model[3] = vec4(vInstancePosition, 1.0);
    gl_Position = projectionMatrix * viewMatrix * model * vec4(vPosition, 1.0);
}
