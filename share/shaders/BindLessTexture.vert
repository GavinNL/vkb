#version 450

layout(location = 0) out vec3 fragColor;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

layout(push_constant) uniform PushConsts
{
    int texture0;
    int texture1;
} pushC;

//========================================================================
// Bindless Texture
//========================================================================
layout (set = 0, binding = 0) uniform sampler2D  u_TextureArray[1024];

// helper function
vec4 bindlessTexture(int index, vec2 fragTexCoord)
{
    return texture( u_TextureArray[ clamp(index, 0, 1023) ], fragTexCoord);
}
 
//========================================================================

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = colors[gl_VertexIndex];
}

