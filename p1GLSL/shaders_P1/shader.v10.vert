#version 330 core

in vec3 inPos;
in vec3 inColor;
in vec3 inNormal;
in vec2 inTexCoord;
uniform mat4 normal;

out vec3 vertColor;
out vec3 outNormal;
out vec2 texCoord;
flat out int instId;

uniform mat4 modelViewProj;

void main() {
    gl_Position = modelViewProj * vec4(inPos, 1);
    vertColor = inColor;
    vec4 normalAux = normal*vec4(inNormal,0);
    outNormal = vec3(normalAux.xyz);
    texCoord = inTexCoord;
    instId = gl_InstanceID;
}
