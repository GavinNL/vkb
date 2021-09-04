#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 v_POSITION;
layout(location = 1) in vec3 v_NORMAL;
layout(location = 2) in vec4 v_COLOR_0;
layout(location = 3) in vec2 v_TEXCOORD_0;

layout(location = 0) out vec4 f_OUTCOLOR;

//================ END DO NOT MODIFY ===========================

struct Texture_ID
{
    int index;
};


struct KHRMaterial_shader
{
    vec4      baseColorFactor   ;//
    vec4      emissiveFactor    ;// // only the RGB values are used

    float     metallicFactor    ;//
    float     roughnessFactor   ;//
    float     alphaCutoff       ;//
    float     vertexNormalOffset;//

    Texture_ID baseColorTextureIndex;
    Texture_ID normalTextureIndex;
    Texture_ID metallicRoughnessTextureIndex;
    Texture_ID occlusionTextureIndex;

    Texture_ID emissiveTextureIndex;
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


layout (set = 1, binding = 0) uniform sampler2D u_TextureArray[32];
layout (set = 1, binding = 1) uniform samplerCube u_TextureCubeArray[32];
layout (set = 2, binding = 3) uniform texture2D u_Texture2D[64];
layout (set = 2, binding = 4) uniform sampler u_Samplers[64];




vec4 texture(Texture_ID id, vec2 p)
{
    return id.index == -1 ? vec4(1,1,1,1) : texture( u_TextureArray[id.index], p);
}



const float PI = 3.141592;
const float Epsilon = 0.00001;
const int NumLights = 3;
// Constant normal incidence Fresnel factor for all dielectrics.
const vec3 Fdielectric = vec3(0.04);

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
        float alpha   = roughness * roughness;
        float alphaSq = alpha * alpha;

        float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
        return alphaSq / (3.141592653f * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
        return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
        float r = roughness + 1.0;
        float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
        return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
        return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

struct PBROut
{
    vec3  position;
    vec3  normal;
    vec3  albedo;
    vec3  emissive;
    float roughness;
    float metallic;
    float alpha;
};

#define HAS_NORMAL_TEXTURE false

vec3 getNormal()
{
    vec3 inWorldPos = v_POSITION;
    vec2 inUV0      = v_TEXCOORD_0;
    vec3 inNormal   = v_NORMAL;

        // Perturb normal, see http://www.thetenthplanet.de/archives/1180
    vec2 normalTextureCoords = inUV0; // material.normalTexture == 0 ? inUV0 : inUV1
        //vec3 tangentNormal = texture(normalTexture, normalTextureCoords).xyz * 2.0 - 1.0;
    vec3 tangentNormal = vec3(0,0,1);

    if( HAS_NORMAL_TEXTURE )
    {
      //  tangentNormal = texture(normalTexture, normalTextureCoords).xyz * 2.0 - 1.0;
    }

    vec3 q1  = dFdx(inWorldPos);
    vec3 q2  = dFdy(inWorldPos);
    vec2 st1 = dFdx(inUV0);
    vec2 st2 = dFdy(inUV0);

    vec3 N = normalize(inNormal);
    vec3 T = normalize(q1 * st2.t - q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec3 getIrradiance(vec3 normal)
{
    #if defined USE_CUBE_MAPS
        return texture(irradianceTexture, normal).rgb;
    #else
        return vec3(1.0f);
    #endif
}

vec3 getSpecularIrradiance(vec3 specularRadianceVector, float roughness)
{
    #if defined USE_CUBE_MAPS
        // Sample pre-filtered specular reflection environment at correct mipmap level.
        int radianceTextureLevels = textureQueryLevels(radianceTexture);
        vec3 specularIrradiance   = textureLod(radianceTexture, specularRadianceVector, roughness * radianceTextureLevels).rgb;

        return specularIrradiance;
    #else
        return vec3(1.0f);
    #endif
}

vec2 getBRDF(float cosLo, float roughness)
{
    #if defined USE_BRDF
        // Split-sum approximation factors for Cook-Torrance specular BRDF.
        vec2 specularBRDF = texture(specularBRDF_LUT, vec2(cosLo, roughness)).rg;
        return specularBRDF;
    #else
        return vec2(0,0);
    #endif

}

vec3 getCameraPosition()
{
        return vec3(0,0,0);
}

vec4 calcPBR(PBROut pbr)
{
        // Sample input textures to get shading model params.
        vec3 albedo     = pbr.albedo;//texture(albedoTexture, vin.texcoord).rgb;
        float metalness = pbr.metallic;//texture(metalnessTexture, vin.texcoord).r;
        float roughness = pbr.roughness;//texture(roughnessTexture, vin.texcoord).r;
        vec3 position   = pbr.position;//vin.position;
        //--------------------------------------------------------------------------
        // Define these from the UBO
        //--------------------------------------------------------------------------
        vec3 eyePosition = getCameraPosition();
        //--------------------------------------------------------------------------

        // Outgoing light direction (vector from world-space fragment position to the "eye").
        vec3 Lo = normalize(eyePosition - position);

        // Get current fragment's normal and transform to world space.
        vec3 N = pbr.normal;


        // Angle between surface normal and outgoing light direction.
        float cosLo = max(0.0, dot(N, Lo));

        // Specular reflection vector.
        vec3 Lr = 2.0 * cosLo * N - Lo;

        // Fresnel reflectance at normal incidence (for metals use albedo color).
        vec3 F0 = mix(Fdielectric, albedo, metalness);


        // Direct lighting calculation for analytical lights.
        vec3 directLighting = vec3(0);


// disable Direct lighting for now
#if 0
        int NumLights = 0;
        for(int i=0; i < NumLights; ++i)
        {
            vec3 Li        = -lights[i].direction;
            vec3 Lradiance =  lights[i].radiance;

            // Half-vector between Li and Lo.
            vec3 Lh = normalize(Li + Lo);

            // Calculate angles between surface normal and various light vectors.
            float cosLi = max(0.0, dot(N, Li));
            float cosLh = max(0.0, dot(N, Lh));

            // Calculate Fresnel term for direct lighting.
            vec3 F  = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
            // Calculate normal distribution for specular BRDF.
            float D = ndfGGX(cosLh, roughness);
            // Calculate geometric attenuation for specular BRDF.
            float G = gaSchlickGGX(cosLi, cosLo, roughness);

            // Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
            // Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
            // To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
            vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);

            // Lambert diffuse BRDF.
            // We don't scale by 1/PI for lighting & material units to be more convenient.
            // See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
            vec3 diffuseBRDF = kd * albedo;

            // Cook-Torrance specular microfacet BRDF.
            vec3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

            // Total contribution for this light.
            directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
        }
#endif

        // Ambient lighting (IBL).
        vec3 ambientLighting = vec3(0.f);

        {
                // Sample diffuse irradiance at normal direction.
                vec3 irradiance = getIrradiance(N);

                // Calculate Fresnel term for ambient lighting.
                // Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
                // use cosLo instead of angle with light's half-vector (cosLh above).
                // See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
                vec3 F = fresnelSchlick(F0, cosLo);

                // Get diffuse contribution factor (as with direct lighting).
                vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);

                // Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
                vec3 diffuseIBL = kd * albedo * irradiance;

                // Sample pre-filtered specular reflection environment at correct mipmap level.
                vec3 specularIrradiance     = getSpecularIrradiance(Lr, roughness);

                // Split-sum approximation factors for Cook-Torrance specular BRDF.
                vec2 specularBRDF = getBRDF(cosLo, roughness);

                // Total specular IBL contribution.
                vec3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

                // Total ambient lighting contribution.
                ambientLighting = diffuseIBL + specularIBL;
        }

        // Final fragment color.
        return vec4(directLighting + ambientLighting, 1.0);
}


//================ END DO NOT MODIFY ===========================


void main()
{
    
    PBROut pbr;
    pbr.albedo.rgb = s_Material.data[pushC.materialIndex0].baseColorFactor.rgb;
    f_OUTCOLOR  = calcPBR(pbr);

    //f_OUTCOLOR = vec4( texture(u_TextureCubeArray[31], v_NORMAL).rgb, 1.0f);//f_Color_0.rgba;
    //f_OUTCOLOR = vec4( texture(u_TextureArray[31], v_TEXCOORD_0).rgb, 1.0f);//f_Color_0.rgba;
}
