#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is
// (size x size). The color of the bottom-left most tile should be "colors[0]"
// and the 2 tiles adjacent to it should have the color "colors[1]".

// TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main() {

  // getting postion of the pixel after normalizing by size
  vec2 Pos = floor(gl_FragCoord.xy / float(size));
  // if the sum of x and y is even then the color is colors[0] else colors[1]
  int mask = mod(Pos.x + mod(Pos.y, 2.0), 2.0);
  // if mask is 1 then the color is colors[1] else colors[0]
  frag_color = vec4(colors[int(mask)], 1.0);
}