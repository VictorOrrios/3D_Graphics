#version 330 core

in vec3 inPos;
in vec3 inNormal;

out vec3 outNormal;

uniform mat4 modelViewProj;

void main() {
    gl_Position = modelViewProj * vec4(inPos, 1);
    outNormal = inNormal;
}
