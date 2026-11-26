#version 330 core

in vec3 inPos;
in vec3 inNormal;
in vec2 inTexCoord;
in vec3 inTangent;

uniform mat4 modelView;
uniform mat4 modelViewProj;
uniform mat4 normal;

out vec3 vNormal;
out vec3 vPos;
out vec2 vTexCoord;
out vec3 vTangent;

void main() {

    vNormal = normalize((normal * vec4(inNormal, 0.0)).xyz);
    vPos = (modelView * vec4(inPos, 1.0)).xyz;
    vTexCoord = inTexCoord;
    vTangent = inTangent;

    gl_Position = modelViewProj * vec4(inPos, 1.0);
}
