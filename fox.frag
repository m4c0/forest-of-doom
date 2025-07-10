#version 450

layout(set = 0, binding = 0) uniform sampler2D tex;

layout(location = 0) in vec2 f_uv;
layout(location = 1) in float f_alpha;

layout(location = 0) out vec4 colour;

void main() {
  colour = textureLod(tex, f_uv * 16, 0) * f_alpha;
}
