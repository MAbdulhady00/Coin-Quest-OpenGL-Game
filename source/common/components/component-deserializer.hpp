#pragma once

#include "../ecs/entity.hpp"
#include "camera.hpp"
#include "mesh-renderer.hpp"
#include "player.hpp"
#include "free-camera-controller.hpp"
#include "player-movement-controller.hpp"
#include "movement.hpp"
#include "score-digit.hpp"
namespace our
{

    // Given a json object, this function picks and creates a component in the given entity
    // based on the "type" specified in the json object which is later deserialized from the rest of the json object
    inline void deserializeComponent(const nlohmann::json &data, Entity *entity)
    {
        std::string type = data.value("type", "");
        Component *component = nullptr;
        if (type == CameraComponent::getID())
        {
            // if the type is "CameraComponent", then create a new CameraComponent and return pointer to it
            component = entity->addComponent<CameraComponent>();
        }
        else if (type == FreeCameraControllerComponent::getID())
        {
            // if the type is "FreeCameraControllerComponent", then create a new FreeCameraControllerComponent and return pointer to it
            component = entity->addComponent<FreeCameraControllerComponent>();
        }
        else if (type == MovementComponent::getID())
        {
            // if the type is "MovementComponent", then create a new MovementComponent and return pointer to it
            component = entity->addComponent<MovementComponent>();
        }
        else if (type == MeshRendererComponent::getID())
        {
            // if the type is "MeshRendererComponent", then create a new MeshRendererComponent and return pointer to it
            component = entity->addComponent<MeshRendererComponent>();
        }
        else if (type == PlayerComponent::getID())
        {
            // if the type is "PlayerComponent", then create a new PlayerComponent and return pointer to it
            component = entity->addComponent<PlayerComponent>();
        }
        else if (type == PlayerMovementControllerComponent::getID())
        {
            // if the type is "PlayerMovementControllerComponent", then create a new PlayerMovementControllerComponent and return pointer to it
            component = entity->addComponent<PlayerMovementControllerComponent>();
        }
        else if (type == ScoreDigitComponent::getID())
        {
            // if the type is "ScoreDigitComponent", then create a new ScoreDigitComponent and return pointer to it
            component = entity->addComponent<ScoreDigitComponent>();
        }
        else
        {
            // if the type is not recognized, then return
            return;
        }
        if (component)
            // if the component is not null, then deserialize it
            component->deserialize(data);
    }

}