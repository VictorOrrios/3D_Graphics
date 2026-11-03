#version 330 core

in vec3 vertColor;
in vec2 texCoord;
uniform sampler2D colorTex;

out vec4 outColor;

void main() {
    vec4 color = texture2D(colorTex, texCoord);
    outColor = color;
}
