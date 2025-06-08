#include "scene.h"
#include <numbers>
#include <cmath>
#include <SDL3/SDL_rect.h>
#include <imgui.h>

static float time = 0.0f;
static float rpm = 60.0f;
constexpr float PI = std::numbers::pi_v<float>;

void Scene::draw(SDL_Renderer& renderer, float delta_time) const {
    time += delta_time;
    SDL_FRect rect;

    SDL_SetRenderDrawColor(&renderer, 125, 200, 255, SDL_ALPHA_OPAQUE);  /* blue, full alpha */
    rect.x = 100.0f + 200.0f * std::cos(time * 2.0f * PI * rpm / 60.0f);
    rect.y = 100.0f + 200.0f * std::sin(time * 2.0f * PI * rpm / 60.0f);
    rect.w = 440.0f;
    rect.h = 280.0f;
    SDL_RenderFillRect(&renderer, &rect);
}

void Scene::draw_gui() {
    ImGuiWindowFlags window_flags = 0;

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Elevator Controls", NULL, window_flags)) {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    ImGui::End();
}
