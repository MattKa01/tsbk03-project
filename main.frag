#version 330

uniform vec2 Resolution;
in vec2 outPos;
out vec4 fragColor;

// Simple height function for terrain
float heightFunc(float x, float z) {
    return sin(x) * sin(z);
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
    //vec2 uv = gl_FragCoord.xy - 0.5 * Resolution / Resolution.y;
    vec2 uv = outPos.xy;

    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}