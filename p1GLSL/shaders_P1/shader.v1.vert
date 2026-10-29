#version 330 core

in vec3 inPos;

void main() {

    // Frustrum params
    float n = 1;
    float f = 11;
    float fov = 60;

    // Near plane sizes
    float r = n * tan(radians(fov / 2.0));
    float l = -r;
    float t = r;
    float b = -t;

    // View matrix
    mat4 view = mat4(1.0f);
    view[3].z = -3;

    // Projection matrix. Column major
    mat4 proj = mat4(0.0f);
    proj[0].x = 2 * n / (r - l);
    proj[1].y = 2 * n / (t - b);
    proj[2] = vec4(
            (r + l) / (r - l),
            (t + b) / (t - l),
            -(f + n) / (f - n),
            -1);
    proj[3].z = -2 * f * n / (f - n);

    gl_Position = proj * view * vec4(inPos, 1);
}
