#pragma once

#include "./light.hpp"

namespace our
{

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class LightSpectrumComponent : public LightComponent
    {
        glm::vec3 hueShift(glm::vec3 color, float hue) const;

    public:
        glm::vec3 getColor(float time);

        static std::string getID() { return "LightSpectrum"; }
    };
} // namespace our