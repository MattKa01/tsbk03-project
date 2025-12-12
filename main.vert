#version 330

layout (location = 0) in vec3 inPos;
in mat4 lookAt;
in mat4 projection;

out vec2 outPos;
void main() {
    vec4 pos = vec4(inPos, 1.0);
    gl_Position = pos;
    outPos = vec2(pos.x,pos.y);
}