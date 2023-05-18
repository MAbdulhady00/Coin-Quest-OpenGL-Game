#pragma once

// player component
namespace our
{
    class PlayerComponent : public Component
    {
    public:
        int score = 0;
        int lives = 3;
        static std::string getID() { return "Player"; }
        void deserialize(const nlohmann::json &data) override
        {
            if (!data.is_object())
                return;
            score = data.value("score", score);
            lives = data.value("lives", lives);
            return;
        }
    };
}