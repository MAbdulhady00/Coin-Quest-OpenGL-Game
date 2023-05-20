#pragma once

#include "../ecs/entity.hpp"
#include "camera.hpp"
#include "mesh-renderer.hpp"
#include "player.hpp"
#include "free-camera-controller.hpp"
#include "light.hpp"
#include "light_spectrum.hpp"
#include "movement.hpp"
#include "player-movement-controller.hpp"
#include "score-digit.hpp"
#include "collision.hpp"
#include "postprocess.hpp"
#include "tags/coin.hpp"
#include "tags/obstacle.hpp"
#include "tags/powerup.hpp"
#include "tags/heart.hpp"
#include "tags/blur-post-process.hpp"
#include "tags/warn-post-process.hpp"

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
        else if (type == LightComponent::getID())
        {
            component = entity->addComponent<LightComponent>();
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
        else if (type == CoinTagComponent::getID())
        {
            // if the type is "CoinTagComponent", then create a new CoinTagComponent and return pointer to it
            component = entity->addComponent<CoinTagComponent>();
        }
        else if (type == ObstacleTagComponent::getID())
        {
            // if the type is "ObstacleTagComponent", then create a new ObstacleTagComponent and return pointer to it
            component = entity->addComponent<ObstacleTagComponent>();
        }
        else if (type == PowerupTagComponent::getID())
        {
            // if the type is "ObstacleTagComponent", then create a new ObstacleTagComponent and return pointer to it
            component = entity->addComponent<PowerupTagComponent>();
        }
        else if (type == HeartTagComponent::getID())
        {
            // if the type is "HeartTagComponent", then create a new HeartTagComponent and return pointer to it
            component = entity->addComponent<HeartTagComponent>();
        }
        else if (type == CollisionComponent::getID())
        {
            // if the type is "CollisionComponent", then create a new CollisionComponent and return pointer to it
            component = entity->addComponent<CollisionComponent>();
        }
        else if (type == PostProcessComponent::getID())
        {
            component = entity->addComponent<PostProcessComponent>();
        }
        else if (type == BlurTagComponent::getID())
        {
            component = entity->addComponent<BlurTagComponent>();
        }
        else if (type == WarnTagComponent::getID())
        {
            component = entity->addComponent<WarnTagComponent>();
        }
        else if (type == LightSpectrumComponent::getID())
        {
            component = entity->addComponent<LightSpectrumComponent>();
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