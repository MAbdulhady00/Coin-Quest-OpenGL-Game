#version 330 core


in vec2 tex_coord;

out vec4 frag_color;


uniform sampler2D tex;
float fogDensity = 0.1;
vec3 fogColor = vec3(0.5,0.5,0.5);

void main()
{
    // Sample the color of the current pixel from the scene texture
    vec4 sceneColor = texture(tex, tex_coord);
    
    // Calculate the fog factor based on the distance from the camera
    float fogFactor = exp(-fogDensity * gl_FragCoord.z);
    
    // Mix the scene color with the fog color based on the fog factor
    vec3 finalColor = mix(sceneColor.rgb, fogColor, fogFactor);
    
    // Set the alpha value of the final color to the scene color's alpha
    frag_color = vec4(finalColor, sceneColor.a);
}
