#include "scene.h"
#include <numbers>
#include <cmath>
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <format>

constexpr float PI = std::numbers::pi_v<float>;

static bool elevator_timer_enabled = true;
static int active_algorithm = 1;
static const char* algorithms[] = { "Manual", "LOOK", "SCAN" };

void Scene::update(float delta_time) {
    this->delta_time = delta_time;
    this->total_time += delta_time;

    // run the algorithm selected
    switch (active_algorithm) {
    case 0: {
        // manual, do nothing
        break;
    }
    case 1: {
        Elevator::single_look(elevator);
        break;
    }
    case 2: {
        Elevator::single_scan(elevator);
        break;
    }
    }

    if (elevator_timer_enabled) {
        elevator.stop_timer = std::max(0.0f, elevator.stop_timer - delta_time);
    }

    if (!elevator.is_stopped()) {
        if (elevator.next_floor > elevator.current_floor) {
            elevator.move_dir = Direction::Up;
        }
        else if (elevator.next_floor < elevator.current_floor) {
            elevator.move_dir = Direction::Down;
        }
        if (elevator.next_floor != elevator.current_floor) {
            // move
            if (elevator.move_dir == Direction::Up) {
                elevator.y_position += (delta_time * elevator.move_speed);
                elevator.current_floor = static_cast<int>(std::floor(elevator.y_position));
            }
            else {
                elevator.y_position -= (delta_time * elevator.move_speed);
                elevator.current_floor = static_cast<int>(std::ceil(elevator.y_position));
            }
        }
        if (elevator.next_floor == elevator.current_floor) {
            elevator.set_stopped(true);
        }
    }
    else {
        // if no queued floors, reset the timer
        if (elevator.next_floor == elevator.current_floor) {
            elevator.set_stopped(true);
        }

        // if we do have a queued floor but the elevator is called at its current floor, reset the timer
        if (elevator.is_floor_called(elevator.current_floor)) {
            elevator.set_stopped(true);
        }

        // snap the y_position to the stopped floor
        elevator.y_position = static_cast<float>(elevator.current_floor);

        elevator.call_floor(elevator.current_floor, false);
    }
}

void Scene::draw(SDL_Renderer& renderer) const {
    // draw the floors as line segments dividing up the height of the window
    int floor_count = 1 + elevator.max_floor - elevator.min_floor;
    SDL_Window* window = SDL_GetRenderWindow(&renderer);
    int width = 1280;
    int height = 720;
    SDL_GetWindowSizeInPixels(window, &width, &height);
    float floor_height = (static_cast<float>(height) / static_cast<float>(floor_count));
    SDL_SetRenderDrawColor(&renderer, 220, 220, 220, SDL_ALPHA_OPAQUE);
    // skip the top-most line
    for (int i = 1; i < floor_count; i++) {
        float line_y = i * floor_height;
        SDL_RenderLine(&renderer, 0.0f, line_y, 400.0f, line_y);
    }

    float cab_height = std::min(floor_height, 64.0f);
    float cab_y_pos = height - cab_height - (floor_height * (elevator.y_position - 1.0f));

    SDL_FRect cab_rect{0.0f, cab_y_pos, 48.0f, cab_height};

    SDL_SetRenderDrawColor(&renderer, 125, 200, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(&renderer, &cab_rect);
}



void Scene::draw_gui() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar;

    static bool docked = false;
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode
            | ImGuiDockNodeFlags_NoUndocking;
    ImGuiID dockspace_id = ImGui::GetID("MainDockspace");

    if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {
        float right_dock_percent = 0.6f;
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
        ImGui::Text("Cab position: %f", elevator.y_position);
        ImGui::Text("Current floor: %d", elevator.current_floor);
        ImGui::Text("Next floor: %d", elevator.next_floor);
        ImGui::Text("Move direction: %s", elevator.move_dir == Direction::Up ? "Up" : "Down");
        ImGui::Text("Stop timer %s", elevator_timer_enabled ? "enabled" : "disabled");
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
        ImGui::Combo("Algorithm", &active_algorithm, algorithms, 3);
        ImGui::DragFloat("Stop duration", &elevator.stop_duration, 0.1f, 0.0f, 10.0f, "%.1f");
        ImGui::DragFloat("Cab speed", &elevator.move_speed, 0.1f, 1.0f, 10.0f, "%.1f");

        if (active_algorithm == 0) {
            ImGui::SeparatorText("Manual controls");
            if (ImGui::BeginListBox("Set next floor", ImVec2(100.0f, 0.0f))) {
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

            ImGui::SameLine();

            if (ImGui::BeginListBox("Teleport to floor", ImVec2(100.0f, 0.0f))) {
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
        else {
            elevator_timer_enabled = true;
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
