#pragma once

namespace our
{
    class CoinComponent : public Component
    {
    public:
        static std::string getID() { return "Coin"; }

        void deserialize(const nlohmann::json &data) override
        {

            return;
        }
    };
}