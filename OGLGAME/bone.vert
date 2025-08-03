#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 weight;
layout(location = 2) in ivec4 boneIndex;

layout(std430, binding = 0) buffer bones
{
    mat4 boneMatricies[];
};