#version 300 es
precision mediump float;

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textureSampler;

float screenPxRange()
{
    float pxRange = 3.0;
    vec2 unitRange = vec2(pxRange) / vec2(textureSize(textureSampler, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(TexCoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec3 msd = texture(textureSampler, TexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    FragColor = mix(vec4(0.0, 0.0, 0.0, 0.0), vec4(1.0, 0, 0, 1.0), opacity);
}
