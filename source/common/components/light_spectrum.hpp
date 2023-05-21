#pragma once

#include "./light.hpp"

namespace our
{

    // A light component that has a color that changes over time
    class LightSpectrumComponent : public LightComponent
    {
        glm::vec3 hueShift(glm::vec3 color, float hue) const;

    public:
        // the color of the light at the current time
        glm::vec3 getColor(float time);

        static std::string getID() { return "LightSpectrum"; }
    };
} // namespace our