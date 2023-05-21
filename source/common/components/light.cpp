#include "light.hpp"
#include "../deserialize-utils.hpp"
#include "../ecs/entity.hpp"
#include <iostream>
namespace our
{
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
            direction = data.value("direction", direction);
        }
        else if (typeLocal == "point")
        {
            typeLight = LightType::POINT;
            const nlohmann::json &attenuationData = data.value("attenuation", nlohmann::json::object());
            attenuation.constant = attenuationData.value("constant", 1.0f);
            attenuation.linear = attenuationData.value("linear", 0.0f);
            attenuation.quadratic = attenuationData.value("quadratic", 0.0f);
        }
        else if (typeLocal == "spot")
        {
            typeLight = LightType::SPOT;
            direction = data.value("direction", direction);

            const nlohmann::json &spotAngleData = data.value("spot_angle", nlohmann::json::object());
            spot_angle.inner = glm::radians(spotAngleData.value("inner", 0.0f));
            spot_angle.outer = glm::radians(spotAngleData.value("outer", 0.0f));

            const nlohmann::json &attenuationData = data.value("attenuation", nlohmann::json::object());
            attenuation.constant = attenuationData.value("constant", 1.0f);
            attenuation.linear = attenuationData.value("linear", 0.0f);
            attenuation.quadratic = attenuationData.value("quadratic", 0.0f);
        }
        else if (typeLocal == "sky")
        {
            typeLight = LightType::SKY;

            const nlohmann::json &skyLightData = data.value("sky_light", nlohmann::json::object());
            sky_light.top_color = skyLightData.value("top_color", glm::vec3(0.0f, 0.0f, 0.0f));
            sky_light.middle_color = skyLightData.value("middle_color", glm::vec3(0.0f, 0.0f, 0.0f));
            sky_light.bottom_color = skyLightData.value("bottom_color", glm::vec3(0.0f, 0.0f, 0.0f));
        }
        else
        {
            throw std::runtime_error("Unknown light type " + typeLocal);
        }
    }
} // namespace our
