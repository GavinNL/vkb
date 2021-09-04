#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 in_POSITION  ;
layout(location = 1) in vec3 in_NORMAL    ;
layout(location = 2) in vec3 in_TANGENT   ;
layout(location = 3) in vec2 in_TEXCOORD_0;
layout(location = 4) in vec2 in_TEXCOORD_1;
layout(location = 5) in vec4 in_COLOR_0   ;
layout(location = 6) in uvec4 in_JOINTS_0  ;
layout(location = 7) in vec4 in_WEIGHTS_0 ;

layout(location = 0) out vec3 f_POSITION;
layout(location = 1) out vec3 f_NORMAL;
layout(location = 2) out vec4 f_COLOR_0;
layout(location = 3) out vec2 f_TEXCOORD_0;

//==================== DO NOT MODIFY ===========================

struct KHRMaterial_shader
{
    vec4      baseColorFactor   ;//
    vec4      emissiveFactor    ;// // only the RGB values are used

    float     metallicFactor    ;//
    float     roughnessFactor   ;//
    float     alphaCutoff       ;//
    float     vertexNormalOffset;//

    int baseColorTextureIndex;
    int normalTextureIndex;
    int metallicRoughnessTextureIndex;
    int occlusionTextureIndex;

    int emissiveTextureIndex;
    int unused0;
    int unused1;
    int unused2;
};

layout(push_constant) uniform PushConsts
{
    uint projViewMatrixIndex;
    uint attributeFlags;  // which vertex attributes are available
    uint viewMatrixIndex;
    uint projMatrixIndex;

    uint modelMatrixIndex;
    uint materialIndex0;
    uint materialIndex1;
    uint materialIndex2;
} pushC;

layout(set=0, binding=0) uniform GLOBAL_UNIFORM_t
{
    vec2  VIEWPORT_OFFSET;
    vec2  VIEWPORT_DIM;

    vec2  MOUSE_POS;
    float TIME_DELTA;
    int   FRAME_NUMBER;

} GLOBAL_UNIFORM;

layout(set=0, binding = 1) buffer readonly s_Transform_t
{
    mat4 data[];
} s_Transform;

layout(set=0, binding = 2) buffer readonly s_Material_t
{
    KHRMaterial_shader data[];
} s_Material;

out gl_PerVertex
{
    vec4 gl_Position;
};

//================ END DO NOT MODIFY ===========================

void main()
{
    KHRMaterial_shader Material = s_Material.data[pushC.materialIndex0];

    mat4 modelMatrix    = s_Transform.data[pushC.modelMatrixIndex + gl_InstanceIndex];
    mat4 projViewMatrix = s_Transform.data[pushC.projViewMatrixIndex + pushC.materialIndex2];

    vec3 normal_world   = normalize(transpose(inverse(mat3( modelMatrix ))) * in_NORMAL);
    vec4 position_world = modelMatrix * vec4( in_POSITION, 1.0);

    gl_Position  = projViewMatrix * position_world;

    f_POSITION   = position_world.xyz;
    f_NORMAL     = normal_world;
    f_TEXCOORD_0 = in_TEXCOORD_0;
    f_COLOR_0    = in_COLOR_0;
}
