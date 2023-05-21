#include "light_spectrum.hpp"

namespace our
{
    // hue shift the given color by the given angle
    // this is done by shifting the color in the hue channel by the given angle hue
    inline glm::vec3 LightSpectrumComponent::hueShift(glm::vec3 color, float hue) const
    {
        // precomputed axis of rotation
        static const glm::vec3 k = glm::vec3(0.57735, 0.57735, 0.57735);
        float cosAngle = glm::cos(hue);
        // see https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
        return glm::vec3(color * cosAngle + glm::cross(k, color) * glm::sin(hue) + k * glm::dot(k, color) * (1.0f - cosAngle));
    }

    glm::vec3 LightSpectrumComponent::getColor(float time)
    {
        // color after hue shift
        return hueShift(color, time);
    }
}