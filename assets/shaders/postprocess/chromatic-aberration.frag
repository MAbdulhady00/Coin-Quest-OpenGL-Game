#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

// How far (in the texture space) is the distance (on the x-axis) between
// the pixels from which the red/green (or green/blue) channels are sampled
#define STRENGTH 0.005

// Chromatic aberration mimics some old cameras where the lens disperses light
// differently based on its wavelength. In this shader, we will implement a
// cheap version of that effect 

void main(){
    //TODO: Modify this shader to apply chromatic abberation
    // To apply this effect, we only read the green channel from the correct pixel (as defined by tex_coord)
    // To get the red channel, we move by amount STRENGTH to the left then sample another pixel from which we take the red channel
    // To get the blue channel, we move by amount STRENGTH to the right then sample another pixel from which we take the blue channel
    // Read the green channel from the current pixel
    vec4 green = texture(tex, tex_coord);

    // Read the red channel from the pixel to the left
    vec4 red = texture(tex, tex_coord - vec2(STRENGTH, 0));

    // Read the blue channel from the pixel to the right
    vec4 blue = texture(tex, tex_coord + vec2(STRENGTH, 0));

    // Write the final color
    frag_color.r = red.r;
    frag_color.g = green.g;
    frag_color.b = blue.b;
    frag_color.a = 1.0;


    // frag_color = texture(tex, tex_coord);
}