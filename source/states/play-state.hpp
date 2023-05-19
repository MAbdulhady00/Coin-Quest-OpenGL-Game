#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <systems/player-movement-controller.hpp>
#include <systems/hud.hpp>
#include <systems/collision.hpp>
#include <systems/generator.hpp>

#include "../common/components/player.hpp"
#include <asset-loader.hpp>

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State
{

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    // our::CoinGeneratorSystem coinGeneratorSystem;
    our::HUDSystem hudSystem;
    // our::ObstacleSystem obstacleSystem;
    // our::GemsGeneratorSystem gemGeneratorSystem;
    our::CollisionSystem collisionSystem;
    our::GeneratorSystem generatorSystem;

    void onInitialize() override
    {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets"))
        {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world"))
        {
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
        // We initialize the generator system
        generatorSystem.initialize(config.value("generator", nlohmann::json()));
        collisionSystem.init();
    }

    void onDraw(double deltaTime) override
    {
        // Here, we just run a bunch of systems to control the world logic
        // coinGeneratorSystem.update(&world, (float)deltaTime);
        // gemGeneratorSystem.update(&world, (float)deltaTime);
        // obstacleSystem.update(&world, (float)deltaTime);
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        playerMovementController.update(&world, (float)deltaTime);
        collisionSystem.update(&world, (float)deltaTime);
        hudSystem.update(&world, (float)deltaTime);
        generatorSystem.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        for (auto entity : world.getEntities())
        {
            // Get player component
            our::PlayerComponent *player = entity->getComponent<our::PlayerComponent>();
            if (player && player->lives <= 0)
            {
                getApp()->changeState("menu");
                break;
            }
        }

        // Get a reference to the keyboard object
        auto &keyboard = getApp()->getKeyboard();

        if (keyboard.justPressed(GLFW_KEY_ESCAPE))
        {
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }

        world.deleteMarkedEntities();
    }

    void onDestroy() override
    {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};