#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

uniform float time;

const int kernelSize = 5; // Size of the blur kernel

void main() {

  float blurRadius = abs((1 - 2 * tex_coord.x)) * log(time);
  vec2 texelSize = 1.0 / textureSize(tex, 0);
  vec2 blurStep = blurRadius * texelSize;

  vec4 colorSum = vec4(0.0);
  float weightSum = 0.0;

  // Apply the blur by sampling neighboring texels
  for (int i = -kernelSize; i <= kernelSize; i++) {
    for (int j = -kernelSize; j <= kernelSize; j++) {
      vec2 offset = vec2(i, j) * blurStep;
      vec2 absOffset = abs(offset);
      vec4 sample = texture(tex, tex_coord + offset);

      // Modify the weight calculation based on the absolute x-coordinate
      float weight = 1.0 - length(offset) / blurRadius;
      weight *= 1.0 + absOffset.x * 0.5;

      // Increase weight for side blur
      weight *= 1.0 + absOffset.y * 0.5;

      colorSum += sample * weight;
      weightSum += weight;
    }
  }

  frag_color = colorSum / weightSum;
}
