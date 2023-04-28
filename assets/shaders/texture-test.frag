#version 330 core

// input from the vertex shader
in Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} fs_in;

// output color
out vec4 frag_color;

// texture sampler
uniform sampler2D tex;

void main(){
    // from the texture at the received texture coordinates
    frag_color = texture(tex, fs_in.tex_coord);
}