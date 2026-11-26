// Disney 2012 PBR shader with only diffuse and specular terms
// No sheen, no clearcoat and no subsurface scattering (blend)
// SKELETON: complete the sections marked as TO DO

#version 330 core

const float PI = 3.141592653589793;
const float EPSILON = 1e-5;
const int MATERIAL_COUNT = 2;

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D emiTex;

// Per object properties (Material BRDF definition)
struct DisneyMaterial {
    vec3 baseColor; // Albedo
    float subsurface; // Subsurface scattering blend
    float metallic; // 0 = Dielectric, 1 = Metal
    float specular; // Specular amount (dielectric specular scale)
    float specularTint; // Tint specular towards base color
    float roughness; // Surface roughness
    float sheen; // Velvet-like sheen
    float sheenTint; // Tint for sheen
    float clearcoat; // Clearcoat layer
    float clearcoatGloss; // Clearcoat glossiness
};

const int POINT_LIGHT = 0;
const int SPOT_LIGHT = 2;

struct Light {
    vec3 pos;
    vec3 color;
    float intensity;
    int type;
    vec3 direction;
    float spot_radius;
};

// Per light definition
const int LIGHT_COUNT = 3;
const Light lights[LIGHT_COUNT] = Light[](
        Light(vec3(0.0, 0.0, 15.0), vec3(1.0), 1.0, 0, vec3(0.0),0.0),
        Light(vec3(-3.0, 0.0, -3.0), vec3(0.0, 0.0, 1.0), 1.0, 0, vec3(0.0),0.0),
        Light(vec3(0.0, 5.0, -6.0), vec3(1.0, 0.0, 0.0), 5.0, 2, vec3(0,-1,0),0.97)
    );
vec3 Ia = vec3(0.0);

out vec4 outColor;

// D: GTR2 (GGX / Trowbridge-Reitz) normal distribution function
float D_GGX(float NdotH, float alpha) {
    float a2 = alpha * alpha;
    float denom = (NdotH * NdotH) * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;
    return a2 / max(denom, EPSILON);
}

// F: Schlick Fresnel approximation
vec3 F_Schlick(vec3 F0, float cosTheta) {
    // Schlick: F = F0 + (1 - F0) * (1 - cosTheta)^5
    float pow5 = pow(1.0 - cosTheta, 5.0);
    return F0 + (1.0 - F0) * pow5;
}

// Schlick-GGX geometry helper for one direction
float G_Schlick_GGX(float NdotV, float k) {
    return NdotV / (NdotV * (1.0 - k) + k);
}

// G: Smith geometry term using Schlick-GGX
float G_Smith(float NdotV, float NdotL, float k) {
    return G_Schlick_GGX(NdotV, k) * G_Schlick_GGX(NdotL, k);
}

// Schlick scalar for sheen
float F_SchlickScalar(float F0, float cosTheta) {
    return 0.0;
}

// GTR1 used by Disney for clearcoat (normalized)
float D_GTR1(float NdotH, float alpha) {
    return 0.0;
}

// Main Disney BRDF evaluation
vec3 disneyBRDF(DisneyMaterial m, vec3 N, vec3 V, vec3 L) {
    vec3 brdfColor = vec3(0.0);

    vec3 H = normalize(L + V);
    float NdotL = clamp(dot(N, L), 0.0, 1.0); // cos(theta_l)
    float NdotV = clamp(dot(N, V), 0.0, 1.0); // cos(theta_v)
    float NdotH = clamp(dot(N, H), 0.0, 1.0); // cos(theta_h)
    float VdotH = clamp(dot(V, H), 0.0, 1.0); // cos(theta_d)

    // Specular term (GGX). TO DO
    // Use the isotropic microfacet model: (D * F * G) / (4 * NdotL * NdotV)
    // Remember that F0 depends on the metallic parameter
    float alpha = max(m.roughness * m.roughness, EPSILON);
    float D = D_GGX(NdotH, alpha);

    vec3 tint = normalize(m.baseColor);
    vec3 specularColor = mix(vec3(1.0), tint, m.specularTint);
    vec3 dielectric_F0 = vec3(0.04) * m.specular * specularColor;

    vec3 F0 = mix(dielectric_F0, m.baseColor, m.metallic);
    vec3 F = F_Schlick(F0, VdotH);

    float k = m.roughness + 1.0;
    k = (k * k) / 8.0;
    float G = G_Smith(NdotV, NdotL, k);

    vec3 specular = D * F * G / max(4 * NdotL * NdotV, EPSILON);
    brdfColor += specular;

    // Diffuse term. TO DO
    // Disney diffuse: retro-reflection controlled by roughness
    // Remember that metals do not have a diffuse (Lambertian) component
    float FD_90 = 0.5 + 2.0 * m.roughness * pow(VdotH, 2.0);
    float FV = pow(1.0 - NdotV, 5.0);
    float FL = pow(1.0 - NdotL, 5.0);
    float fdV = 1.0 + (FD_90 - 1.0) * FV;
    float fdL = 1.0 + (FD_90 - 1.0) * FL;
    vec3 diffuse = m.baseColor / PI * (fdV * fdL);
    brdfColor += diffuse * (1.0 - m.metallic);

    // Sheen term
    vec3 sheen = vec3(0.0);
    brdfColor += sheen;

    // Clearcoat term
    vec3 clearcoat = vec3(0.0);
    if (m.clearcoat > 0.0) {
        brdfColor += clearcoat;
    }

    // Subsurface term (approx — slight retro-reflection mix)
    if (m.subsurface > 0.0) {
        float LdotH = clamp(dot(L, H), 0.0, 1.0);
        // diffuse = mix(diffuse, ?, clamp(m.subsurface, 0.0, 1.0));
    }

    return brdfColor * NdotL;
}

// PBR Disney 2012 shading
vec3 shade(DisneyMaterial m);

void main() {
    // Hard-coded palette: Array of DisneyMaterial
    int selectedMaterial = 0;
    const DisneyMaterial materials[MATERIAL_COUNT] = DisneyMaterial[](
            // 0: Brushed Gold (metallic)
            DisneyMaterial(vec3(1.00, 0.84, 0.40), 0.0, 1.0, 0.0, 0.5, 0.24, 0.0, 0.0, 0.0, 0.0),
            // 1: Soft Plastic Toy (dielectric)
            DisneyMaterial(vec3(0.95, 0.3, 0.05), 0.0, 0.0, 0.64, 0.75, 0.15, 0.0, 0.0, 0.0, 0.0));

    outColor = vec4(shade(materials[selectedMaterial]), 1.0);
}

vec3 shade(DisneyMaterial m) {
    vec3 shading = vec3(0.0);

    vec3 N = normalize(vNormal);
    vec3 V = normalize(-vPos);

    // Ambient term. TO DO
    vec3 ambient = Ia * m.baseColor * (1.0 - m.metallic);
    shading += ambient;

    // Specular term + Diffuse term. TO DO
    // Evaluate BRDF (per-light). For multiple lights sum each light contribution
    // The BRDF is already multiplied inside by NdotL (cosine factor)
    for (int i = 0; i < LIGHT_COUNT; i++) {
        vec3 PtoL = lights[i].pos - vPos;
        float L_distance = length(PtoL);
        vec3 L;
        float intensity;
        switch(lights[i].type){
            case POINT_LIGHT:
            L = normalize(PtoL);
            intensity = lights[i].intensity/(L_distance*L_distance);
            break;

            case SPOT_LIGHT:
            L = normalize(PtoL);
            float cos = dot(L,-lights[i].direction);
            if(cos<lights[i].spot_radius) continue;
            intensity = lights[i].intensity;
            break;
        }
        vec3 brdfCos = disneyBRDF(m, N, V, L);
        shading += intensity * lights[i].color * brdfCos;
    }

    // Emissive term
    shading += texture(emiTex, vTexCoord).rgb;

    // Gamma-correct to sRGB. TO DO
    shading = pow(clamp(shading, 0.0, 1.0), vec3(1.0 / 2.2));

    return shading;
}
