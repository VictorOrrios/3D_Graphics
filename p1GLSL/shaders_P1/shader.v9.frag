#version 330 core

in vec3 vertColor;
in vec3 outNormal;

out vec4 outColor;

void main() {
    vec4 color = vec4(outNormal, 1);
    outColor = color;
}
