#version 330
// we can using include using the stb library
#include "light_phong.glsl"

#define MAX_LIGHTS 8

uniform Light lights[MAX_LIGHTS];
uniform int light_count;

uniform Sky sky;

uniform Material material;

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;

out vec4 frag_color;

void main() {
    // normalize the normal and view vector
    vec3 normal = normalize(fs_in.normal);
    vec3 view = normalize(fs_in.view);
    
    // compute the sky light, this will be a more realistic ambient light
    vec3 ambient_light = compute_sky_light(sky, normal);

    // sample the textures and compute the material properties
    vec3 diffuse = texture(material.albedo, fs_in.tex_coord).rgb;
    vec3 specular = texture(material.specular, fs_in.tex_coord).rgb;
    float roughness = texture(material.roughness, fs_in.tex_coord).r;
    vec3 ambient = diffuse * texture(material.ambient_occlusion, fs_in.tex_coord).r;
    vec3 emissive = texture(material.emissive, fs_in.tex_coord).rgb;
    float shininess = 2.0 / pow(clamp(roughness, 0.001, 0.999), 4.0) - 2.0;
    // constant lighting terms
    vec3 color = emissive + ambient_light * ambient;

    vec3 world_to_light_dir;
    // loop over all the lights
    for(int light_idx = 0; light_idx < min(MAX_LIGHTS, light_count); light_idx++){
        Light light = lights[light_idx];
        float attenuation = 1.0;
        // Directional light
        if(light.type == DIRECTIONAL){
            world_to_light_dir = -light.direction;
        } 
        // Spot and point lights
        else {
            // calculate the direction from the object to the light
            world_to_light_dir = light.position - fs_in.world;
            float d = length(world_to_light_dir);
            // normalize the vector
            world_to_light_dir /= d;

            // calculate the attenuation factor based on the quadratic, linear and constant factors
            attenuation = 1.0 / dot(light.attenuation, vec3(d*d, d, 1.0));

            // Spot light
            if(light.type == SPOT){
                // calculate the angle between the light direction and the direction from the object to the light
                float angle = acos(dot(light.direction, -world_to_light_dir));
                // if the angle is greater than the outer cone angle, the light is not visible
                // if the angle is smaller than the inner cone angle, the light is fully visible
                // if the angle is between the inner and outer cone angle, the light is partially visible
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle);
            }
        }

        // calculate the diffuse and specular components
        vec3 computed_diffuse = light.color * diffuse * lambert(normal, world_to_light_dir);
        vec3 reflected = reflect(-world_to_light_dir, normal);
        vec3 computed_specular = light.color * specular * phong(reflected, view, shininess);

        // blend the computed light 
        color += (computed_diffuse + computed_specular) * attenuation;
    }
    
    frag_color = vec4(color, 1.0);
}