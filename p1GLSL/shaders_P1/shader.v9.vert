#version 330 core

in vec3 inPos;
in vec3 inNormal;
uniform mat4 normal;

out vec3 outNormal;

uniform mat4 modelViewProj;

void main() {
    gl_Position = modelViewProj * vec4(inPos, 1);
    vec4 normalAux = normal*vec4(inNormal,0);
    outNormal = vec3(normalAux.xyz);
}
