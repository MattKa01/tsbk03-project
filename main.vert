#version 330

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inUV;

out vec2 outUV;

void main() {
    gl_Position = vec4(inPos,0.0f, 1.0f);
    outUV = inUV;
}