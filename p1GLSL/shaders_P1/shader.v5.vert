#version 330 core

in vec3 inPos;

out vec3 vertColor;

uniform mat4 modelViewProj;

void main() {
    gl_Position = modelViewProj * vec4(inPos, 1);
    vertColor = vec3(
        (gl_VertexID % 8) / 8.0,
        ((gl_VertexID + 3) % 8) / 8.0,
        ((gl_VertexID + 5) % 8) / 8.0);
}
