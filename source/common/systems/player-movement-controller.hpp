#pragma once

#include "../ecs/world.hpp"
#include "../components/player.hpp"
#include "../components/player-movement-controller.hpp"
#include "../components/movement.hpp"
#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The Player controller system is responsible for moving every entity which contains a PlayerComponent.
    class PlayerMovementControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            PlayerComponent *player = nullptr;
            PlayerMovementControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                player = entity->getComponent<PlayerComponent>();
                controller = entity->getComponent<PlayerMovementControllerComponent>();
                if (player && controller)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(player && controller))
                return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = player->getOwner();

            MovementComponent* movement = entity->getComponent<MovementComponent>();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked)
            {
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
                // If the left mouse button is released, we unlock and unhide the mouse.
            }
            // on the other hand if the mouse is locked and the left mouse button is not pressed, we unlock and unhide the mouse
            else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked)
            {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // We get a reference to the entity's position
            glm::vec3 &position = entity->localTransform.position;

            // we get the TRS matrix of the entity to get the front, up and right vectors
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));
            
            glm::vec3 current_sensitivity = controller->positionSensitivity;

            // get the jump speed
            float jumpSpeed = controller->jumpSpeed;

            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            current_sensitivity *= controller->speedupFactor;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth

            // D || Right Arrow moves the player right
            if (app->getKeyboard().isPressed(GLFW_KEY_D) || app->getKeyboard().isPressed(GLFW_KEY_RIGHT))
                position += right * (deltaTime * current_sensitivity.x);
            // A || Left Arrow moves the player left
            if (app->getKeyboard().isPressed(GLFW_KEY_A) || app->getKeyboard().isPressed(GLFW_KEY_LEFT))
                position -= right * (deltaTime * current_sensitivity.x);
            // W || Up Arrow moves the player up
            if((app->getKeyboard().isPressed(GLFW_KEY_W) || app->getKeyboard().isPressed(GLFW_KEY_UP)) && movement->linearVelocity.y == 0.0f)
                movement->linearVelocity.y = jumpSpeed;
                
            // clamp x position to -10 and 10
            if (position.x > controller->maxHorizontalDistance)
                position.x = controller->maxHorizontalDistance;
            if (position.x < -controller->maxHorizontalDistance)
                position.x = -controller->maxHorizontalDistance;
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }
    };
}
