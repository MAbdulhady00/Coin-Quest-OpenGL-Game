#version 330 core

// Input from vertex shader
in Varyings {
  vec4 color;
  vec2 tex_coord;
}
fs_in;

// Output color
out vec4 frag_color;

// Ting color
uniform vec4 tint;
// Texture
uniform sampler2D tex;

void main() {
  
  // multiplying the tint with the vertex color and with the texture color 
  frag_color = tint * fs_in.color * texture(tex, fs_in.tex_coord);
}