#version 330


layout (location = 0) in vec3 pos;

out float height;
out vec4 vColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    height = sin(pos.x) + cos(pos.z);
    gl_Position = projection * view * model * vec4(pos.x, height, pos.z, 1.0f);
    vColor = vec4(0.2f, 0.4f, 0.9f, 0.5f);
}