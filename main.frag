#version 330
in vec2 outUV;
uniform vec2 resolution;
out vec4 fragColor;

// Simple height function for terrain
float heightFunc(float x, float z) {
    return sin(x) * sin(z);
}

float sdSphere(vec3 p, float r) {
    return length(p) - r;
}

// Ray marching function to find intersection with terrain
bool castRay(vec3 rayOrigin, vec3 rayDir, out float resT) {
    float minDist = 0.001f;
    float maxDist = 10.0f;
    float stepSize = 0.01f;

    // Linear interpolation variables
    float lh = 0.0;
    float ly = 0.0;
    for (float t = minDist; t < maxDist; t += stepSize) {
        vec3 p = rayOrigin + rayDir * t;
        float terrainHeight = heightFunc(p.x, p.z);
        if (p.y < terrainHeight) {
            // Interpolate intersection distance
            resT = t-stepSize+stepSize*(lh-ly)/(p.y-ly-terrainHeight+lh);
            return true;
        }
        // Accuracy proportionate to distance
        stepSize = 0.01 * t;
        lh = terrainHeight;
        ly = p.y;
    }
    return false;
}

void main() {
    //vec2 uv = (2.0*outUV - resolution.xy) / resolution.y;
    vec2 uv = outUV;
    uv.x *= resolution.x / resolution.y;

    vec3 col = vec3(uv * 0.5f + 0.5f, 0.0f);
    fragColor = vec4(abs(uv.x), abs(uv.y), 0.0, 1.0);
}