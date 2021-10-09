#version 450

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;


layout(push_constant) uniform PushConsts
{
    int texture0;
    int texture1;
} pushC;

//========================================================================
// Bindless Texture
//========================================================================
layout (set = 0, binding = 0) uniform sampler2D    u_TextureArray[1024];
layout (set = 0, binding = 1) uniform samplerCube  u_TextureCubeArray[1024];

// helper function
vec4 bindlessTexture(int index, vec2 fragTexCoord)
{
    return texture( u_TextureArray[ clamp(index, 0, 1023) ], fragTexCoord);
}
vec4 bindlessTextureCube(int index, vec3 fragTexCoord)
{
    return texture( u_TextureCubeArray[ clamp(index, 0, 1023) ], fragTexCoord);
}
 
//========================================================================

void main() {
    outColor = bindlessTextureCube( pushC.texture0, vec3(0,0,2));
    outColor = bindlessTexture( pushC.texture0, vec2(0,0));
}

