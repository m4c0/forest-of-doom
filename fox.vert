#version 450

layout(location = 0) in vec2 v_pos;
layout(location = 1) in vec2 pos;

void main() {
  vec2 p = v_pos + pos;

  gl_Position = vec4(p, 0, 1);
}
