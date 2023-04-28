#version 330 core

// input from vertex buffer
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

// output to fragment shader
out Varyings { vec4 color; }
vs_out;

// transformation matrix
uniform mat4 transform;

void main() {

  // apply the transformation matrix
  gl_Position = transform * vec4(position, 1.0);
  // set the color
  vs_out.color = color;
}