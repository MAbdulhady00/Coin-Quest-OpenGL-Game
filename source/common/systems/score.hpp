#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/movement.hpp"
#include "../components/coin.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/camera.hpp"
#include "../components/player.hpp"
#include "../components/heart.hpp"
#include "../components/score-digit.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <imgui_impl/imgui_impl_opengl3.h>
#include <audio/audio.hpp>
namespace our
{

    class ScoreSystem
    {
        int currentScore = 0;
        int currentLives = 3;

    public:
        void init()
        {
            currentScore = 0;
            currentLives = 3;
        }
        void setScoreDigitTexture(MeshRendererComponent *Component, int score)
        {
            Component->material = AssetLoader<Material>::get("score" + std::to_string(score));
        }

        int getLives()
        {
            return currentLives;
        }

        // This should be called every frame to update all entities containing a CoinComponent.
        void update(World *world, float deltaTime)
        {
            // set seed for random number generator to be the current time
            srand(time(NULL));
            glm::vec3 playerPosition = glm::vec3(0, 0, 0);
            // For each entity in the world get the player position
            for (auto entity : world->getEntities())
            {
                // Get the camera and controller components to get the player position
                PlayerComponent *player = entity->getComponent<PlayerComponent>();
                if (player)
                    playerPosition = player->getOwner()->localTransform.position;
            }

            // For each entity in the world get the coins
            for (auto entity : world->getEntities())
            {
                // check if the entity has a CoinComponent
                if (entity->getComponent<CoinComponent>())
                {
                    // If the coin passed the player, remove it
                    if (entity->localTransform.position.z > playerPosition.z + 1.0f)
                    {
                        world->markForRemoval(entity);
                        printf("Removed Coin was too far\n");
                    }
                    // If the coin is close to the player, remove it and count a point
                    if (glm::distance(playerPosition, entity->localTransform.position) < 1.0f)
                    {
                        world->markForRemoval(entity);
                        currentScore++;
                        printf("Current Score : %d\n", currentScore);
                    }
                }
                if (entity->getComponent<ObstacleComponent>())
                {
                    // If the obstacle is close to the player, remove it and count a point
                    if (glm::distance(playerPosition, entity->localTransform.position) < 1.0f)
                    {
                        currentLives--;
                    }
                }

                if (entity->getComponent<HeartComponent>())
                {
                    // If the obstacle is close to the player, remove it and count a point
                    if (glm::distance(playerPosition, entity->localTransform.position) < 1.0f)
                    {
                        currentLives++;
                    }
                }
            }
            // // Update Score Digits
            // for (auto entity : world->getEntities())
            // {
            //     ScoreDigitComponent* scoreDigit = entity->getComponent<ScoreDigitComponent>();
            //     if(scoreDigit)
            //     {
            //         if (scoreDigit->digit == "hundreds")
            //         {
            //             setScoreDigitTexture(entity->getComponent<MeshRendererComponent>(), currentScore / 100);
            //         }
            //         else if (scoreDigit->digit == "tenths")
            //         {
            //             setScoreDigitTexture(entity->getComponent<MeshRendererComponent>(), (currentScore / 10) % 10);
            //         }
            //         else if (scoreDigit->digit == "units")
            //         {
            //             setScoreDigitTexture(entity->getComponent<MeshRendererComponent>(), currentScore % 10);
            //         }
            //     }
            // }

            // test drawing text using imgui
            // ImGui::GetFont()->Scale = 4.0f;
            // ImGui::PushFont(ImGui::GetFont());
            // ImDrawList *drawList = ImGui::GetBackgroundDrawList();
            // drawList->AddText(ImVec2(25, 25), ImColor(1.0f, 1.0f, 0.0f), std::to_string(currentScore).c_str());
            // drawList->AddText(ImVec2(1230, 25), ImColor(1.0f, 1.0f, 0.0f), std::to_string(currentLives).c_str());
            // ImGui::PopFont();
            // Delete all the marked entities
            world->deleteMarkedEntities();
        }
    };

}
