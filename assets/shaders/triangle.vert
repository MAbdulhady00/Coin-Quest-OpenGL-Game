#version 330

// This vertex shader should be used to render a triangle whose normalized
// device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader
// where the colors are (in order): (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0,
// 0.0, 1.0)

// Varyings 
out Varyings { vec3 color; }
vs_out;

// Currently, the triangle is always in the same position, but we don't want
// that. So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is
// (1.0, 1.0).
// This vector of size 2 represents scale(size) with default value (1, 1).
uniform vec2 scale = vec2(1, 1);
// This vector of size 2 represents translation(position) with default value (0, 0).
uniform vec2 translation = vec2(0, 0);

void main() {
  // The positions of the vertices of the triangle
  const vec3 positions[3] =
      vec3[3](vec3(-0.5, -0.5, 0.0), vec3(0.5, -0.5, 0.0), vec3(0.0, 0.5, 0.0));
  // The colors of the vertices of the triangle
  const vec3 colors[3] =
      vec3[3](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));

  // get the position based on the vertex id
  vec3 position = positions[gl_VertexID];
  // Scale it
  position.xy *= scale;
  // Translate it
  position.xy += translation;
  // set the position
  gl_Position = vec4(position, 1.0);
  // set the color
  vs_out.color = colors[gl_VertexID];
}