#version 330 core

// inputs of the vertex shader
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;

// output of the vertex shader
out Varyings {
  vec4 color;
  vec2 tex_coord;
}
vs_out;

// transformation matrix
uniform mat4 transform;

void main() {
  // Apply transformation matrix

  // multiply the position by the transform matrix to get the final position
  gl_Position = transform * vec4(position, 1.0);
  // pass the color and tex_coord to the fragment shader
  vs_out.color = color;
  vs_out.tex_coord = tex_coord;
}