#include "light_spectrum.hpp"

namespace our
{
    inline glm::vec3 LightSpectrumComponent::hueShift(glm::vec3 color, float hue) const
    {
        static const glm::vec3 k = glm::vec3(0.57735, 0.57735, 0.57735);
        float cosAngle = glm::cos(hue);
        return glm::vec3(color * cosAngle + glm::cross(k, color) * glm::sin(hue) + k * glm::dot(k, color) * (1.0f - cosAngle));
    }

    glm::vec3 LightSpectrumComponent::getColor(float time)
    {
        return hueShift(color, time);
    }
}