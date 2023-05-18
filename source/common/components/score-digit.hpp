#pragma once

#include "../ecs/component.hpp"
// Score Digit component
namespace our
{
    class ScoreDigitComponent : public Component
    {
    public:
        std::string digit = "units";
        static std::string getID() { return "Score Digit"; }
        void deserialize(const nlohmann::json &data) override;
    };
}