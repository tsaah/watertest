#version 460 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNorm;
// layout (location = 2) in vec4 aTangent;
// layout (location = 3) in vec3 aColor;
// layout (location = 4) in vec2 aUV;

// out vec3 oPos;
// out vec3 oNorm;
// out vec4 oTangent;
// out vec3 oColor;
// out vec2 oUV;

// uniform sampler2D tex0;
// uniform sampler2D tex1;
// uniform sampler2D tex2;
// uniform sampler2D tex3;

// layout (std140, binding = 0) uniform CameraBlock {
//     mat4 view;
//     mat4 projection;
//     vec4 up;
//     vec4 position;
//     vec4 direction;
//     float aspectRatio;
//     float near;
//     float far;
//     float fov;
//     float pitch;
//     float yaw;
//     int width;
//     int height;
// } camera;

// vec2 pointOnASphereToUV(vec3 p) {
//     p = normalize(p);
//     const float longitude = atan(p.x, -p.z);
//     const float latitude = asin(p.y);

//     const float pi = 3.1415926f;
//     return vec2(0.5f - (1.0f + longitude / pi) * 0.5f, latitude / pi + 0.5f);
// }

void main() {
    // oPos = aPos;
    // oNorm = aNorm;
    // oTangent = aTangent;
    // oColor == aColor;
    // oUV = aUV;

    // // vec2 coord = asin(point.y), atan(point.x, -point.z) };
    // const float height = 6378.137f + texture(tex0, pointOnASphereToUV(aPos)).r * 8.848f * 20;

    // gl_Position = camera.projection * camera.view * vec4(aPos + normalize(aPos) * height, 1);
    gl_Position = vec4(aPos, 1);
    // gl_Position = camera.projection * camera.view * vec4(aPos, 1);
}

