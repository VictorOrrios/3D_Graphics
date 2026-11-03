#version 330 core

in vec3 vertColor;
in vec2 texCoord;
uniform sampler2D colorTex;

out vec4 outColor;

void main() {
    float aux1 = texCoord.x-0.5f;
    float aux2 = texCoord.y-0.5f;
    bool discardCondition = aux1*aux1 + aux2*aux2 > 0.2;

    if(discardCondition)
        discard;

    vec4 color = texture2D(colorTex, texCoord);
    outColor = color;
}
