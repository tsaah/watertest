#version 460 core

layout (quads, equal_spacing, ccw) in;

out vec3 oPos;

uniform sampler2D tex0;

layout (std140, binding = 0) uniform CameraBlock {
    mat4 view;
    mat4 projection;
    vec4 up;
    vec4 position;
    vec4 direction;
    float aspectRatio;
    float near;
    float far;
    float fov;
    float pitch;
    float yaw;
    int width;
    int height;
} camera;

vec2 pointOnASphereToUV(vec3 p) {
    p = normalize(p);
    const float longitude = atan(p.x, -p.z);
    const float latitude = asin(p.y);

    const float pi = 3.1415926f;
    return vec2(0.5f - (1.0f + longitude / pi) * 0.5f, latitude / pi + 0.5f);
}

vec3 displaced(vec3 p, float amount) {
    const vec3 t = vec3(
        p.x * p.x,
        p.y * p.y,
        p.z * p.z
    );
    p = vec3(
        p.x * sqrt(1.0f - (t.y + t.z) / 2.0f + (t.y * t.z) / 3),
        p.y * sqrt(1.0f - (t.z + t.x) / 2.0f + (t.z * t.x) / 3),
        p.z * sqrt(1.0f - (t.x + t.y) / 2.0f + (t.x * t.y) / 3)
    );

    const float height = 6378.137f + texture(tex0, pointOnASphereToUV(p)).r * 8.848f * amount;
    return normalize(p) * height;
}

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 pos0 = gl_in[0].gl_Position;
    vec4 pos1 = gl_in[1].gl_Position;
    vec4 pos2 = gl_in[2].gl_Position;
    vec4 pos3 = gl_in[3].gl_Position;

    vec4 leftPos = pos0 + v * (pos3 - pos0);
    vec4 rightPos = pos1 + v * (pos2 - pos1);
    vec4 pos = leftPos + u * (rightPos - leftPos);

    gl_Position = camera.projection * camera.view * vec4(displaced(pos.xyz, 100), 1);
    oPos = pos.xyz;
}