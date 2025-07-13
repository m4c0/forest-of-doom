#version 450

layout(push_constant) uniform upc {
  vec2 grid_pos;
  vec2 grid_size;
};

layout(location = 0) in vec2  v_pos;
layout(location = 1) in vec2  pos;
layout(location = 2) in vec2  uv;
layout(location = 3) in vec2  size;
layout(location = 4) in float alpha;
layout(location = 5) in uint  texid;

layout(location = 0) out vec2  f_uv;
layout(location = 1) out float f_alpha;
layout(location = 2) out uint  f_texid;

void main() {
  vec2 p = (v_pos * size + pos - grid_pos) / grid_size;

  gl_Position = vec4(p, 0, 1);
  f_uv = uv + v_pos * size;
  f_alpha = alpha;
  f_texid = texid;
}
