#pragma once

namespace our
{
    class ObstacleComponent : public Component
    {
    public:
        static std::string getID() { return "Obstacle"; }

        void deserialize(const nlohmann::json &data) override
        {
            // No data to deserialize
            return;
        }
    };
}