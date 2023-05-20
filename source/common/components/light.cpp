#include "light.hpp"
#include "../deserialize-utils.hpp"
#include "../ecs/entity.hpp"
#include <iostream>
namespace our
{
    // Reads linearVelocity & angularVelocity from the given json object
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        std::string typeLocal = data.value("typeLight", "");
        enabled = data.value("enabled", enabled);
        color = data.value("color", color);
        if (typeLocal == "directional")
        {
            typeLight = LightType::DIRECTIONAL;
            direction = data["direction"];
        }
        else if (typeLocal == "point")
        {
            typeLight = LightType::POINT;
            attenuation.constant = data["attenuation"]["constant"];
            attenuation.linear = data["attenuation"]["linear"];
            attenuation.quadratic = data["attenuation"]["quadratic"];
        }
        else if (typeLocal == "spot")
        {
            typeLight = LightType::SPOT;
            direction = data["direction"];
            attenuation.constant = data["attenuation"]["constant"];
            attenuation.linear = data["attenuation"]["linear"];
            attenuation.quadratic = data["attenuation"]["quadratic"];
            spot_angle.inner = glm::radians(data["spot_angle"].value("inner", 0.0f));
            spot_angle.outer = glm::radians(data["spot_angle"].value("outer", 0.0f));
        }
        else if (typeLocal == "sky")
        {
            typeLight = LightType::SKY;
            sky_light.top_color = data["sky_light"]["top_color"];
            sky_light.bottom_color = data["sky_light"]["bottom_color"];
            sky_light.middle_color = data["sky_light"]["middle_color"];
        }
        else
        {
            throw std::runtime_error("Unknown light type " + typeLocal);
        }
    }
} // namespace our
