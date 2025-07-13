#version 450

layout(constant_id = 99) const uint tex_count = 2;

layout(set = 0, binding = 0) uniform sampler2D tex[tex_count];

layout(location = 0) in vec2 f_uv;
layout(location = 1) in float f_alpha;
layout(location = 2) in flat uint f_texid;

layout(location = 0) out vec4 colour;

void main() {
  colour = textureLod(tex[f_texid], f_uv * 16, 0) * f_alpha;
}
