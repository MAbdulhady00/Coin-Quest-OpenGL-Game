#pragma once

#include "../ecs/component.hpp"
#include <glm/gtx/euler_angles.hpp>

namespace our
{

    enum class LightType
    {
        DIRECTIONAL,
        POINT,
        SPOT,
        SKY
    };
    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class LightComponent : public Component
    {
    public:
        LightType typeLight;
        bool enabled = true;
        // Note that we removed the 3 components and replaced it with color.
        // This is a bit more realistic since light color shouldn't differ between diffuse and specular.
        // But you may want to keep them separate if you want extra artistic control where you may want to ignore realism.
        // Also, we no longer have an ambient term in the  We will keep the ambient in a separate struct called "SkyLight".

        glm::vec3 color = {1.0f, 1.0f, 1.0f}; // The color of the light, we do not need separate diffuse and specular colors because it is unrealistic.
        glm::vec3 position;                   // Used for Point and Spot Lights only
        glm::vec3 direction;                  // Used for Directional and Spot Lights only
        struct
        {
            float quadratic, linear, constant;
        } attenuation; // Used for Point and Spot Lights only
        struct
        {
            float inner, outer;
        } spot_angle; // Used for Spot Lights only
        struct
        {
            glm::vec3 top_color, middle_color, bottom_color;
        } sky_light;

        // The ID of this component type is "Movement"
        static std::string getID() { return "Light"; }
        virtual std::string getIDPolymorphic() { return getID(); }
        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json &data) override;
    };
} // namespace our