#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/movement.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/camera.hpp"
#include "../components/player.hpp"
#include "../components/tags/heart.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <imgui_impl/imgui_impl_opengl3.h>

namespace our
{

    class HUDSystem
    {
    public:
        // This should be called every frame to update the HUD(Heads Up Display)
        void update(World *world, float deltaTime)
        {
            PlayerComponent *player = nullptr;
            // For each entity in the world get the player position
            for (auto entity : world->getEntities())
            {
                // Get player component
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

            // Set the font scale
            ImGui::GetFont()->Scale = 4.0f;
            // Push the font
            ImGui::PushFont(ImGui::GetFont());
            // Get the draw list
            ImDrawList *drawList = ImGui::GetBackgroundDrawList();
            // Draw the score, lives and distance on the screen
            glm::vec3 playerPosition = player->getOwner()->getWorldTranslation();
            drawList->AddText(ImVec2(25, 25), ImColor(1.0f, 1.0f, 0.0f), ("Score: " + std::to_string(player->score)).c_str());
            drawList->AddText(ImVec2(1000, 25), ImColor(1.0f, 1.0f, 0.0f), ("Lives: " + std::to_string(player->lives)).c_str());
            drawList->AddText(ImVec2(500, 25), ImColor(1.0f, 1.0f, 0.0f), ("Distance: " + std::to_string(abs((long long)playerPosition.z))).c_str());
            ImGui::PopFont();
        }
    };

}
