#version 330 core

in vec3 vertColor;
in vec3 outNormal;
in vec2 texCoord;
uniform sampler2D colorTex;

out vec4 outColor;

vec2 n22(vec2 p)
{
    vec3 a = fract(p.xyx * vec3(123.34, 234.34, 345.65));
    a += dot(a, a + 34.45);
    return fract(vec2(a.x * a.y, a.y * a.z));
}

// From https://www.shadertoy.com/view/DsK3W1
vec2 get_gradient(vec2 pos)
{
    float twoPi = 6.283185;
    float angle = n22(pos).x * twoPi;
    return vec2(cos(angle), sin(angle));
}

// From https://www.shadertoy.com/view/DsK3W1
float perlin_noise(vec2 uv, float cells_count)
{
    vec2 pos_in_grid = uv * cells_count;
    vec2 cell_pos_in_grid = floor(pos_in_grid);
    vec2 local_pos_in_cell = (pos_in_grid - cell_pos_in_grid);
    vec2 blend = local_pos_in_cell * local_pos_in_cell * (3.0f - 2.0f * local_pos_in_cell);

    vec2 left_top = cell_pos_in_grid + vec2(0, 1);
    vec2 right_top = cell_pos_in_grid + vec2(1, 1);
    vec2 left_bottom = cell_pos_in_grid + vec2(0, 0);
    vec2 right_bottom = cell_pos_in_grid + vec2(1, 0);

    float left_top_dot = dot(pos_in_grid - left_top, get_gradient(left_top));
    float right_top_dot = dot(pos_in_grid - right_top, get_gradient(right_top));
    float left_bottom_dot = dot(pos_in_grid - left_bottom, get_gradient(left_bottom));
    float right_bottom_dot = dot(pos_in_grid - right_bottom, get_gradient(right_bottom));

    float noise_value = mix(
            mix(left_bottom_dot, right_bottom_dot, blend.x),
            mix(left_top_dot, right_top_dot, blend.x),
            blend.y);

    return (0.5 + 0.5 * (noise_value / 0.7));
}

void main() {
    vec4 color;
    int objId = gl_PrimitiveID/(4); // Cubes with 4 vertex per face

    if (objId == 0) {
        float aux1 = texCoord.x - 0.5f;
        float aux2 = texCoord.y - 0.5f;
        bool discardCondition = aux1 * aux1 + aux2 * aux2 > 0.2;

        if (discardCondition)
            discard;
        color = vec4(vec3(perlin_noise(texCoord, 10)), 1);

    }else if (objId == 1) {
        color = vec4(outNormal, 1);
    }else{
        color = texture2D(colorTex, texCoord);
    }
    outColor = color;
}
