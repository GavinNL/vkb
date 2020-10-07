#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 f_Position;
layout(location = 1) in vec3 f_Normal;
layout(location = 2) in vec4 f_Color_0;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = f_Color_0.rgba;
}
