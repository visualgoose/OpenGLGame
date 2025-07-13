#version 460 core

layout (location = 0) out vec4 fOutCol;

uniform sampler2D tex;

uniform vec2 viewportSize;

void main()
{
    vec2 uvCoord = gl_FragCoord.xy / viewportSize;
    fOutCol = texture(tex, uvCoord);
}