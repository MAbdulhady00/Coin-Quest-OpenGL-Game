#pragma once

#include "../ecs/component.hpp"

namespace our
{
    class CoinComponent : public Component
    {

    public:
        int score = 1;
        void deserialize(const nlohmann::json &data)
        {
            if (!data.is_object())
                return;
            score = data.value("score", score);
        }
        static std::string getID() { return "Coin"; }
    };
}