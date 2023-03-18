#version 330 core

// This shader is designed to work with "triangle.vert" and it receives an
// interpolated varying which represents the vertex color.

in Varyings {
    vec3 color;
} fs_in;

out vec4 frag_color;

// currently the shader just returns the interpalated color varying.
// However, we want to mix the color channels around. We can do this using a 
// color matrix which we will send to the shader as 3 uniforms: red, green, blue.
// Each of these 3 variables will be a vec4. To apply the channel mixing for a
// certain channel (e.g. red), we apply this linear transformation:
// frag_color.r = red.r * fs_in.color.r + red.g + fs_in.color.g + red.b + fs_in.color.b + red.a;
// However, this line is too long to write, so we can simplify it using a dot product
// (which is defined in the "dot" function).

//TODO: (Req 1) Finish this shader and apply the channel mixing using the "dot" function.

// creating 3 uniforms for the color matrix
uniform vec4 red;
uniform vec4 green;
uniform vec4 blue;

void main(){

    // create a vec4 for each of the color channels
    frag_color.r = dot(red.rgb, fs_in.color) + red.a;
    frag_color.g = dot(green.rgb, fs_in.color) + green.a;
    frag_color.b = dot(blue.rgb, fs_in.color)+ blue.a;
}