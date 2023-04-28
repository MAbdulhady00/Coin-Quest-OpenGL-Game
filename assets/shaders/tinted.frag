#version 330 core

// Input vertex data.
in Varyings { vec4 color; }
fs_in;

// fragment color
out vec4 frag_color;

// Tint color
uniform vec4 tint;

void main() {
  
  // Multiply the tint color by the fragment color
  frag_color = tint * fs_in.color;
}