#version 300 es
precision mediump float;

out vec4 FragColor;

// uniform vec4 vColor;
in vec2 TexCoord;

uniform sampler2D textureSampler;

void main()
{
    FragColor = texture(textureSampler, TexCoord);
}