#pragma once

namespace our
{
    class HeartComponent : public Component
    {
    public:
        static std::string getID() { return "Heart"; }

        void deserialize(const nlohmann::json &data) override
        {
            // No data to deserialize
            return;
        }
    };
}