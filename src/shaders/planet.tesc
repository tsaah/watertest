#version 460 core

layout (vertices = 4) out;

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

const int MIN_TES = 1;
const int MAX_TES = 32;
const float MIN_DIST = 0;
const float MAX_DIST = 1000;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    // oPos[gl_InvocationID] = aPos[gl_InvocationID];
    // oNorm[gl_InvocationID] = aNorm[gl_InvocationID];
    // oTangent[gl_InvocationID] = aTangent[gl_InvocationID];
    // oColor[gl_InvocationID] == aColor[gl_InvocationID];
    // oUV[gl_InvocationID] = aUV[gl_InvocationID];
    if (gl_InvocationID == 0) {
        vec3 center0 = displaced(gl_in[0].gl_Position.xyz + (gl_in[3].gl_Position.xyz - gl_in[0].gl_Position.xyz) / 2.0, 100); // left side
        vec3 center1 = displaced(gl_in[1].gl_Position.xyz + (gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz) / 2.0, 100); // bot side
        vec3 center2 = displaced(gl_in[2].gl_Position.xyz + (gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz) / 2.0, 100); // right side
        vec3 center3 = displaced(gl_in[3].gl_Position.xyz + (gl_in[2].gl_Position.xyz - gl_in[3].gl_Position.xyz) / 2.0, 100); // top side

        float dist0 = length(camera.position.xyz - center0);
        float dist1 = length(camera.position.xyz - center1);
        float dist2 = length(camera.position.xyz - center2);
        float dist3 = length(camera.position.xyz - center3);

        int tes0 = int(mix(MAX_TES, MIN_TES, clamp(dist0 / MAX_DIST, 0.0, 1.0)));
        int tes1 = int(mix(MAX_TES, MIN_TES, clamp(dist1 / MAX_DIST, 0.0, 1.0)));
        int tes2 = int(mix(MAX_TES, MIN_TES, clamp(dist2 / MAX_DIST, 0.0, 1.0)));
        int tes3 = int(mix(MAX_TES, MIN_TES, clamp(dist3 / MAX_DIST, 0.0, 1.0)));



        gl_TessLevelOuter[0] = tes0;
        gl_TessLevelOuter[1] = tes1;
        gl_TessLevelOuter[2] = tes2;
        gl_TessLevelOuter[3] = tes3;

        gl_TessLevelInner[0] = max(tes1, tes3);
        gl_TessLevelInner[1] = max(tes0, tes2);
    }
}