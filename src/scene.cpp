#include "scene.h"
#include <numbers>
#include <cmath>
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <format>

static float rpm = 60.0f;
static bool elevator_timer_enabled = true;
constexpr float PI = std::numbers::pi_v<float>;

void Scene::update(float delta_time) {
    this->delta_time = delta_time;
    this->total_time += delta_time;

    if (elevator_timer_enabled) {
        elevator.stop_timer = std::max(0.0f, elevator.stop_timer - delta_time);
    }

    if (elevator.next_floor > elevator.current_floor) {
        elevator.move_dir = Direction::Up;
    }
    else if (elevator.next_floor < elevator.current_floor) {
        elevator.move_dir = Direction::Down;
    }
}

void Scene::draw(SDL_Renderer& renderer) const {
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

        // elevator status
        ImGui::SeparatorText("Status");
        ImGui::Text("Current floor: %d", elevator.current_floor);
        ImGui::Text("Next floor: %d", elevator.next_floor);
        ImGui::Text("Move direction: %s", elevator.move_dir == Direction::Up ? "Up" : "Down");
        ImGui::Text("Stop timer: %f", elevator.stop_timer);
        ImGui::Text("Stopped: %s", elevator.is_stopped() ? "true" : "false");

        // elevator call controls (user-facing)
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

        // backend controls
        ImGui::SeparatorText("Backend");
        static int active_algorithm = 0;
        static const char* algorithms[] = { "Manual", "LOOK", "SCAN" };
        ImGui::Combo("Algorithm", &active_algorithm, algorithms, 3);
        ImGui::DragFloat("Stop duration", &elevator.stop_duration, 0.1f, 0.0f, 10.0f, "%.1f");

        if (active_algorithm == 0) {
            ImGui::SeparatorText("Manual controls");
            if (ImGui::BeginListBox("Set next floor")) {
                for (int i = elevator.max_floor; i >= elevator.min_floor; i--) {
                    const bool is_selected = (elevator.next_floor == i);
                    if (ImGui::Selectable(std::format("Floor {}", i).c_str(), is_selected)) {
                        elevator.next_floor = i;
                    }

                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndListBox();
            }

            if (ImGui::BeginListBox("Teleport to floor")) {
                for (int i = elevator.max_floor; i >= elevator.min_floor; i--) {
                    const bool is_selected = (elevator.current_floor == i);
                    if (ImGui::Selectable(std::format("Floor {}", i).c_str(), is_selected)) {
                        elevator.current_floor = i;
                        elevator.y_position = static_cast<float>(i);
                    }

                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndListBox();
            }

            ImGui::Checkbox("Enable stop timer", &elevator_timer_enabled);
            if (ImGui::Button("Toggle start/stop")) {
                elevator.set_stopped(!elevator.is_stopped());
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
