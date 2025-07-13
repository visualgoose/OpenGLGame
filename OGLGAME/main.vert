#version 450 core

in vec3 vInPosition;

void main()
{
    gl_Position = vec4(vInPosition, 1.0f);
}