#include "score-digit.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads digit from the given json object
    void ScoreDigitComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        digit = data.value("digit", digit);
    }
}