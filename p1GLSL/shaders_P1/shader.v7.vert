#version 330 core

in vec3 inPos;
in vec3 inColor;
in vec2 inTexCoord;

out vec3 vertColor;
out vec2 texCoord;

uniform mat4 modelViewProj;

void main() {
    gl_Position = modelViewProj * vec4(inPos, 1);
    vertColor = inColor;
    texCoord = inTexCoord;
}
