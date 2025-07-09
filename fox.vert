#version 450

layout(push_constant) uniform upc {
  vec2 grid_size;
};

layout(location = 0) in vec2 v_pos;
layout(location = 1) in vec2 pos;

void main() {
  vec2 p = (v_pos + pos) / grid_size;

  gl_Position = vec4(p, 0, 1);
}
