#version 460 core

in vec3 oPos;
// in vec3 oNorm;
// in vec4 oTangent;
// in vec3 oColor;
// in vec2 oUV;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

uniform float seaLevel = 0.5f;

layout (location = 0) out vec4 outColor;

vec2 pointOnASphereToUV(vec3 p) {
    p = normalize(p);
    const float longitude = atan(p.x, -p.z);
    const float latitude = asin(p.y);

    const float pi = 3.1415926f;
    return vec2(0.5f - (1.0f + longitude / pi) * 0.5f, latitude / pi + 0.5f);
}

void main() {
    // vec2 coord = asinf(point.y), atan(point.x, -point.z) };
    const float height = texture(tex0, pointOnASphereToUV(oPos)).r;

    const float realHeight = -10894 + (7891 + 10894) * height;

    // outColor = vec4(0.605f, 0.46875f, 0.46875f, 1);
    // outColor = vec4(oNorm, 1);
    // outColor = vec4(oUV, 0, 1);
    // outColor = texture(tex0, oUV);

    vec4 color = vec4(height, height, height, 1) * vec4(0.7f, 1.0f, 0.7f, 1.0f);
    if (height < seaLevel) {
        color = vec4(height, height, height, 1) * vec4(0.7f, 0.7f, 1.0f, 1.0f);
    }


    outColor = color;
    // gl_FragCoord = vec4(gl_FragCoord.xy, 1, 1);

    // outColor = vec4(0.605f, 0.46875f, 0.46875f, 1);
}
