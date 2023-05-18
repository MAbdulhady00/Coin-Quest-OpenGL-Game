#pragma once

#include "../ecs/world.hpp"
#include "../ecs/entity.hpp"
#include "../components/collision.hpp"
#include "../components/player.hpp"
#include "../components/tags/coin.hpp"
#include "../components/tags/heart.hpp"
#include "../components/tags/obstacle.hpp"
#include <audio/audio.hpp>
#include <glm/glm.hpp>

namespace our
{

    class CollisionSystem
    {
        AudioPlayer audioPlayer;

        /**
         * This function is called when the player collides with a coin or a heart.
         * It returns true if the collided entity should be destroyed.
         */
        inline bool onCollision(PlayerComponent *playerComponent, Entity *collidedEntity)
        {
            // If the collided entity is a coin
            if (collidedEntity->getComponent<CoinTagComponent>())
            {
                // Increase the score
                playerComponent->score++;
            }
            // If the collided entity is a heart
            if (collidedEntity->getComponent<HeartTagComponent>())
            {
                // Increase the lives
                playerComponent->lives++;
            }
            // If the collided entity is an obstacle
            if (collidedEntity->getComponent<ObstacleTagComponent>())
            {
                // Decrease the lives
                playerComponent->lives--;
            }

            // play sound effect
            audioPlayer.play(collidedEntity->getComponent<CollisionComponent>()->soundPath,
                             collidedEntity->getComponent<CollisionComponent>()->soundName);
            return true;
        }

    public:
        void init()
        {
            audioPlayer.init();
        }
        // This should be called every frame to update all entities containing a CollisionComponent.
        void update(World *world, float deltaTime)
        {
            glm::vec3 playerPosition = glm::vec3(0, 0, 0);
            PlayerComponent *player = nullptr;
            // For each entity in the world get the player position
            for (auto entity : world->getEntities())
            {
                // Get the player position
                player = entity->getComponent<PlayerComponent>();
                if (player)
                {
                    break;
                }
            }

            if (!player)
            {
                return;
            }

            playerPosition = player->getOwner()->localTransform.position;

            CollisionComponent *collision = nullptr;
            // For each entity in the world get the coins
            for (auto entity : world->getEntities())
            {
                // check if the entity has a CoinComponent
                collision = entity->getComponent<CollisionComponent>();
                if (collision)
                {
                    // Check if the player is colliding with the coin
                    if (glm::distance(playerPosition, entity->localTransform.position) <= collision->detectionRadius)
                    {
                        // handle collision
                        if (onCollision(player, entity))
                            world->markForRemoval(entity);
                    }
                }
            }
        }
    };

}
