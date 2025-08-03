#version 460 core

layout (location = 0) in vec3 vi_position;
layout (location = 1) in vec2 vi_texCoord;

layout (location = 0) out vec2 vo_texCoord;

uniform mat4 u_oglgame_mvp;

void main()
{
    vo_texCoord = vi_texCoord;
    gl_Position = u_oglgame_mvp * vec4(vi_position, 1.0f);
}