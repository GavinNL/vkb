/**
*
* This is a very simple shader for testing. It is a constant shader
* that always produces a coloured triangle. It doesn't require any
* vertex attributes, or uniform buffers.
*/

#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec4 in_Color_0;

layout(location = 0) out vec3 f_Position;
layout(location = 1) out vec3 f_Normal;
layout(location = 2) out vec4 f_Color_0;

layout(push_constant) uniform PushConsts
{
    mat4 model;
    mat4 proj;
} pushC;


out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    f_Position   = (pushC.model * vec4( in_Position, 1.0)).xyz;
    f_Normal     = in_Normal;
    f_Color_0    = in_Color_0;
    gl_Position  = pushC.proj * vec4( f_Position, 1.0);
}
