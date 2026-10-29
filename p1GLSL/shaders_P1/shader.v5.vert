#version 330 core

in vec3 inPos;
in vec3 inColor;

out vec3 vertColor;

uniform mat4 modelViewProj;

void main() {
    gl_Position = modelViewProj * vec4(inPos, 1);
    vertColor = inColor;
}
