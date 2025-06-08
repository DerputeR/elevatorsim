#include "scene.h"
#include <numbers>
#include <cmath>
#include <SDL3/SDL_rect.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <format>

static float total_time = 0.0f;
static float rpm = 60.0f;
constexpr float PI = std::numbers::pi_v<float>;

void Scene::draw(SDL_Renderer& renderer, float delta_time) const {
    total_time += delta_time;
    SDL_FRect rect;

    SDL_SetRenderDrawColor(&renderer, 125, 200, 255, SDL_ALPHA_OPAQUE);
    rect.x = 100.0f + 100.0f * std::cos(total_time * 2.0f * PI * rpm / 60.0f);
    rect.y = 100.0f + 100.0f * std::sin(total_time * 2.0f * PI * rpm / 60.0f);
    rect.w = 64.0f;
    rect.h = 64.0f;
    SDL_RenderFillRect(&renderer, &rect);
}

void Scene::draw_gui() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;

    static bool docked = false;
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode
            | ImGuiDockNodeFlags_NoUndocking;
    ImGuiID dockspace_id = ImGui::GetID("MainDockspace");

    if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {
        float right_dock_percent = 0.25f;
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, main_viewport->WorkSize);
        ImGuiID right_dock_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, right_dock_percent, nullptr, nullptr);
        ImGui::DockBuilderDockWindow("Elevator Controls", right_dock_id);
        ImGui::DockBuilderDockWindow("Dear ImGui Demo", right_dock_id);
        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::DockSpaceOverViewport(dockspace_id, main_viewport, dockspace_flags);

    //ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_Once);
    //ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Once);
    if (ImGui::Begin("Elevator Controls", nullptr, window_flags)) {
        
        // draw controls
        ImGui::SeparatorText("Status");
        ImGui::Text("Current floor: %d", elevator.current_floor);
        ImGui::Text("Next floor: %d", elevator.next_floor);
        ImGui::Text("Move direction: %s", elevator.move_dir == Direction::Up ? "Up" : "Down");
        ImGui::Text("Stopped: %s", elevator.stopped ? "true" : "false");

        ImGui::SeparatorText("Call buttons");
        for (int i = elevator.max_floor; i >= elevator.min_floor; i--) {
            if (ImGui::Button(std::format("Call Floor {}", i).c_str())) {
                elevator.call_floor(i);
            }
            if (elevator.is_floor_called(i)) {
                ImGui::SameLine();
                ImGui::Text("Called");
            }
        }
    }

    // show the demo window only when in debug build
#ifdef DEBUG
    ImGui::ShowDemoWindow();
#endif

    // this mainly only does something if we have multiple things docked
    if (!docked) {
        ImGui::FocusWindow(ImGui::FindWindowByName("Elevator Controls"));
        docked = true;
    }


    ImGui::End();
}
