#include "free-camera-controller.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads sensitivities & speedupFactor from the given json object
    void FreeCameraControllerComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        // Read the rotataion senseitivity 
        rotationSensitivity = data.value("rotationSensitivity", rotationSensitivity);
        // Read the fov sensetivity
        fovSensitivity = data.value("fovSensitivity", fovSensitivity);
        // Read the position sensitivity
        positionSensitivity = data.value("positionSensitivity", positionSensitivity);
        // Read the speedup factor
        speedupFactor = data.value("speedupFactor", speedupFactor);
    }
}