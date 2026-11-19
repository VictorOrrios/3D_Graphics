#version 330 core

in vec3 vNormal;
in vec3 vColor;
in vec3 vPos;

// Per object properties
vec3 ka;
vec3 kd;
vec3 ks;
float alpha;

// Per light definition
vec3 Lpos = vec3(3.0,0.0,3.0);
vec3 Ia = vec3(0.3);
vec3 Id = vec3(1.0);
vec3 Is = vec3(1.0);

out vec4 outColor;

vec3 shade();

void main() {
    ka = vColor;
    kd = vColor;
    ks = vec3(1.0);
    alpha = 500.0;

	outColor = vec4(shade(), 1.0);
}

vec3 shade() {
    vec3 shading = vec3(0.0);
    vec3 N = vNormal;
    vec3 L = normalize(Lpos - vPos);
    vec3 V = normalize(-vPos); // Camera position is 0.0 because of screen space
    // Could also use 4th column of view to get world space camera postion
    vec3 H = normalize(L+V);
    vec3 R = normalize(reflect(-L,N));

    // Ambient term
    vec3 ambient = Ia * ka;
    shading += ambient;

    // Diffuse term
    vec3 diffuse = Id * kd * clamp(dot(R, V),0,1);
    shading += diffuse;

    // Specular term
    float specularFactor = clamp(dot(H,N),0,1);
    vec3 specular = Is * ks * pow(specularFactor,alpha);
    shading += specular;

    return shading;
}
