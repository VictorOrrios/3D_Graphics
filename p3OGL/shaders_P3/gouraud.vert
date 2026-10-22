#version 330 core

in vec3 inPos;
in vec3 inColor;
in vec3 inNormal;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

vec3 vPos;

// Per object properties (Material BRDF definition)
vec3 Ka;
vec3 Kd;
vec3 Ks;
float alpha = 500.0;

// Per light definition
vec3 Lpos = vec3(0.0);
vec3 Ia = vec3(0.3);
vec3 Id = vec3(1.0);
vec3 Is = vec3(1.0);

out vec3 vColor;

// Gouraud shading
vec3 shade();

void main() {
	Ka = inColor;
	Kd = inColor;
	Ks = vec3(1.0);

	vPos = (modelView * vec4(inPos, 1.0)).xyz;
	vColor = shade();

	gl_Position = modelViewProj * vec4(inPos, 1.0);
}

vec3 shade() {
	vec3 shading = vec3(0.0);

	vec3 N = normalize((normal * vec4(inNormal, 0.0)).xyz);
	vec3 L = normalize(Lpos - vPos);
	vec3 V = normalize(-vPos);

	// Ambient term
	vec3 ambient = Ia * Ka;
	shading += ambient;

	// Diffuse term (Lambert)
	vec3 diffuse = Id * Kd * clamp(dot(L, N), 0.0, 1.0);
	shading += diffuse;

	// Specular term
	vec3 R = normalize(reflect(-L, N));
	float specularFactor = clamp(dot(R, V), 0.0, 1.0); 
	vec3 specular = Is * Ks * pow(specularFactor, alpha);
	shading += specular;

	return clamp(shading, 0.0, 1.0);
}