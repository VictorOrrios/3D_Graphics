#version 330 core

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D colorTex;
uniform sampler2D specularTex;
uniform sampler2D emiTex;

// Per object properties (Material BRDF definition)
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;
float alpha = 5000.0;

// Per light definition
vec3 Lpos = vec3(0.0);
vec3 Ia = vec3(0.3);
vec3 Id = vec3(1.0);
vec3 Is = vec3(1.0);

out vec4 outColor;

// Blinn-Phong shading
vec3 shade();

void main() {
	Ka = texture(colorTex, vTexCoord).rgb;
	Kd = texture(colorTex, vTexCoord).rgb;
	Ks = texture(specularTex, vTexCoord).rgb;
	Ke = texture(emiTex, vTexCoord).rgb;

	outColor = vec4(shade(), 1.0);
}

vec3 shade() {
	vec3 shading = vec3(0.0);

	vec3 N = normalize(vNormal);
	vec3 L = normalize(Lpos - vPos);
	vec3 V = normalize(-vPos);

	// Ambient term
	vec3 ambient = Ia * Ka;
	shading += ambient;

	// Diffuse term (Lambert)
	vec3 diffuse = Id * Kd * clamp(dot(L, N), 0.0, 1.0);
	shading += diffuse;

	// Specular term
	vec3 H = normalize(L + V); 
	float specularFactor = clamp(dot(N, H), 0.0, 1.0);
	vec3 specular = Is * Ks * pow(specularFactor, alpha);
	shading += specular;

	// Emissive term
	shading += Ke;

	return clamp(shading, 0.0, 1.0);
}
