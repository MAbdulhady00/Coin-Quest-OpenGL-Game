#pragma once

// player component
namespace our
{
    class PlayerComponent : public Component
    {
    public:
        static std::string getID() { return "Player"; }
        void deserialize(const nlohmann::json &data) override
        {
            // do nothing
            return;
        }
    };
}