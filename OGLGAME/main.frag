#version 460 core

layout (location = 0) out vec4 fo_color;

layout (location = 0) in vec2 fi_texCoord;

uniform sampler2D mainTex;

void main()
{
    fo_color = texture(mainTex, fi_texCoord);
}