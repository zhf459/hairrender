#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 position_v;
out vec4 normal_v;
out vec2 uv_v;
out vec3 color_v;

void main() {
    position_v = model * vec4(position, 1.0);
    normal_v = model * vec4(normal, 0.0);
    uv_v = uv;
    color_v = color;
    gl_Position = projection * view * position_v;
}
