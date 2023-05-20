#pragma once

// player component
namespace our
{
    class PostProcessComponent : public Component
    {
    public:
        int postProcessIndex = 0;
        bool isEnabled = false;
        static std::string getID() { return "PostProcess"; }
        void deserialize(const nlohmann::json &data) override
        {
            if (!data.is_object())
                return;
            isEnabled = data.value("isEnabled", isEnabled);
            postProcessIndex = data.value("postProcessIndex", postProcessIndex);
            return;
        }
    };
}