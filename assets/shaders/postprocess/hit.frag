#version 330

// The texture holding the scene pixels
uniform sampler2D tex;
uniform float time;
// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;

out vec4 frag_color;

// Vignette is a postprocessing effect that darkens the corners of the screen
// to grab the attention of the viewer towards the center of the screen
vec4 col = vec4(0.8, 0.0, 0.0, 1.0);
void main() {
  // TODO: Modify this shader to apply vignette
  //  To apply vignette, divide the scene color
  //  by 1 + the squared length of the 2D pixel location the NDC space
  //  Hint: remember that the NDC space ranges from -1 to 1
  //  while the texture coordinate space ranges from 0 to 1
  //  We have the pixel's texture coordinate, how can we compute its location in
  //  the NDC space?

  // Compute the pixel's location in NDC space
  vec2 ndc_coord = (2.0 * tex_coord) - 1.0;

  // Compute the squared length of the pixel's location in NDC space
  float ndc_length_sq = dot(ndc_coord, ndc_coord);

  // Compute new color
  col = ndc_length_sq * col;

  // Apply the vignette effect to the scene color
  frag_color = texture(tex, tex_coord) + col * pow(sin(2 * time), 2);
}