// Disney 2012 PBR shader with only diffuse and specular terms
// No sheen, no clearcoat and no subsurface scattering (blend)

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
	vec3 baseColor;			// Albedo
	float subsurface; 		// Subsurface scattering blend
	float metallic; 		// 0 = Dielectric, 1 = Metal
	float specular; 		// Specular amount (dielectric specular scale)
	float specularTint; 	// Tint specular towards base color
	float roughness; 		// Surface roughness
	float sheen; 			// Velvet-like sheen
	float sheenTint; 		// Tint for sheen
	float clearcoat;  		// Clearcoat layer
	float clearcoatGloss; 	// Clearcoat glossiness
};

// Per light definition
vec3 Lpos = vec3(0.0, 0.0, 5.0);
vec3 Ia = vec3(0.3);
vec3 lightColor = vec3(1.0);
float lightIntensity = 1.0;

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

    // Specular term (GGX): isotropic microfacet model
	float alpha = max(m.roughness * m.roughness, EPSILON); // Convert roughness to alpha used in D_GGX (Disney uses perceptual mapping)
	float D = D_GGX(NdotH, alpha);

	vec3 tint = normalize(m.baseColor); // normalized hue tint
	vec3 specularColor = mix(vec3(1.0), tint, m.specularTint);
	vec3 dielectricF0 = vec3(0.04) * m.specular * specularColor;

	vec3 F0 = mix(dielectricF0, m.baseColor, m.metallic);
	vec3 F = F_Schlick(F0, VdotH);

	float k = (m.roughness + 1.0);
	k = (k * k) / 8.0;
	float G = G_Smith(NdotV, NdotL, k);

	vec3 specular = (D * F * G) / max(4.0 * NdotL * NdotV + EPSILON, EPSILON);
	brdfColor += specular;

    // Diffuse term: retro-reflection controlled by roughness
	float F_D90 = 0.5 + 2.0 * m.roughness * pow(VdotH, 2.0);
	float FL = pow(1.0 - NdotL, 5.0);
	float FV = pow(1.0 - NdotV, 5.0);
	float fdL = 1.0 + (F_D90 - 1.0) * FL;
	float fdV = 1.0 + (F_D90 - 1.0) * FV;
	vec3 diffuse = m.baseColor / PI * (fdV * fdL);
	brdfColor += diffuse * (1.0 - m.metallic); // Metals do not have a diffuse (Lambertian) component

    // Sheen term
	vec3 sheen = vec3(0.0);
	brdfColor += sheen;

    // Clearcoat term
	vec3 clearcoat = vec3(0.0);
	if(m.clearcoat > 0.0) {
		brdfColor += clearcoat;
	}

    // Subsurface term (approx — slight retro-reflection mix)
	if(m.subsurface > 0.0) {
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
	DisneyMaterial(vec3(1.00, 0.84, 0.40), 0.0, 1.0, 0.0, 0.5, 0.28, 0.0, 0.0, 0.0, 0.0),
    	// 1: Soft Plastic Toy (dielectric)
	DisneyMaterial(vec3(0.95, 0.3, 0.05), 0.0, 0.0, 0.64, 0.75, 0.15, 0.0, 0.0, 0.0, 0.0));

	outColor = vec4(shade(materials[selectedMaterial]), 1.0);
}

vec3 shade(DisneyMaterial m) {
	vec3 shading = vec3(0.0);

	vec3 N = normalize(vNormal);
	vec3 V = normalize(-vPos);
	vec3 L = normalize(Lpos - vPos);

	// Ambient term
	vec3 ambient = Ia * m.baseColor * (1.0 - m.metallic);
	shading += ambient;

	// Specular term + Diffuse term
	// Evaluate BRDF (per-light). For multiple lights sum each light contribution
	vec3 brdf = disneyBRDF(m, N, V, L); // The BRDF is already multiplied inside by NdotL (cosine factor)
	shading += lightIntensity * lightColor * brdf; // Multiply by light properties

	// Emissive term
	shading += texture(emiTex, vTexCoord).rgb;

    // Gamma-correct to sRGB
	shading = pow(clamp(shading, 0.0, 1.0), vec3(1.0 / 2.2));

	return shading;
}
