#version 330 core

in vec3 vNormal;

out vec4 outColor;

void main() {
	outColor = vec4(vNormal, 1.0);
}
