#version 330 core

in vec3 inPos;
in vec3 inColor;
in vec3 inNormal;

uniform mat4 modelView;
uniform mat4 modelViewProj;
uniform mat4 normal;

vec3 vPos;

out vec3 vNormal;

// Per object properties
vec3 ka;
vec3 kd;
vec3 ks;
float alpha = 500.0;

// Per light definition
vec3 Lpos = vec3(3.0,0.0,3.0);
vec3 Ia = vec3(0.3);
vec3 Id = vec3(1.0);
vec3 Is = vec3(1.0);

out vec3 vColor;

vec3 shade();

void main() {
    ka = inColor;
    kd = inColor;
    ks = vec3(1.0);

    vNormal = normalize((normal * vec4(inNormal, 0.0)).xyz);
    vColor = shade();
    vPos = (modelView * vec4(inPos, 1.0)).xyz;

    gl_Position = modelViewProj * vec4(inPos, 1.0);
}

vec3 shade() {
    vec3 shading = vec3(0.0);
    vec3 N = vNormal;
    vec3 L = normalize(Lpos - vPos);
    vec3 V = normalize(-vPos); // Camera position is 0.0 because of screen space
    // Could also use 4th column of view to get world space camera postion
    vec3 H = normalize(L+V);

    // Ambient term
    vec3 ambient = Ia * ka;
    shading += ambient;

    // Diffuse term
    vec3 diffuse = Id * kd * clamp(dot(L, N),0,1);
    shading += diffuse;

    // Specular term
    vec3 specular = Is * ks * clamp(dot(L, H),0,1);
    shading += specular;

    return shading;
}
