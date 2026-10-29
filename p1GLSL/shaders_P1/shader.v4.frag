#version 330 core

out vec4 outColor;

void main() {
    outColor = vec4(
        (gl_PrimitiveID % 8) / 8.0,
        ((gl_PrimitiveID + 3) % 8) / 8.0,
        ((gl_PrimitiveID + 5) % 8) / 8.0,
        1);
}
