#include <iostream>
#include <array>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include "engine/shader_loader.h"

#include "app_info.h"
#include "elevator.h"
#include "scene.h"

static SDL_Window* window;
static SDL_Renderer* renderer; // for ease of drawing stuff until I can convert to pure opengl

static Scene scene{};

static const Uint64 ONE_SECOND_NS = 1000000000;

static bool vsync_enabled = false;
static bool vsync_adaptive = true;
static Uint64 min_frame_time = ONE_SECOND_NS / 120;
static Uint64 time_ns_last = 0;
static Uint64 time_ns_now = 0;
static Uint64 time_ns_delta = 0;

static Uint64 frame_number = 0;

static void update_frame_cap(Uint64 frame_cap) {
    if (frame_cap == 0) {
        min_frame_time = 0;
    }
    else {
        min_frame_time = ONE_SECOND_NS / frame_cap;
    }
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
    SDL_SetAppMetadata(APP_FULL_NAME, APP_VERSION, APP_PACKAGE);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ASSERT, "SDL failed to init: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Window init
    window = SDL_CreateWindow(APP_WINDOW_TITLE, 1280, 720, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Failed to create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // SDL renderer init
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "Failed to create SDL renderer for window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    if (vsync_enabled) {
        if (vsync_adaptive) {
            if (!SDL_SetRenderVSync(renderer, -1)) {
                SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to enable adaptive vsync: %s", SDL_GetError());
                if (!SDL_SetRenderVSync(renderer, 1)) {
                    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to enable vsync: %s", SDL_GetError());
                }
            }
        } else if (!SDL_SetRenderVSync(renderer, 1)) {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to enable vsync: %s", SDL_GetError());
        }
    } else if (!SDL_SetRenderVSync(renderer, 0)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to disable vsync: %s", SDL_GetError());
    }
    
    // Dear ImGUI init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    // compute previous frame time
    time_ns_last = time_ns_now;
    time_ns_now = SDL_GetTicksNS();
    time_ns_delta = time_ns_now - time_ns_last;

    // manual framecap when vsync is off
    if ((!vsync_enabled || (vsync_enabled && vsync_adaptive)) && min_frame_time > 0) {
        
        if (time_ns_delta < min_frame_time) {
            SDL_DelayPrecise(min_frame_time - time_ns_delta);
            time_ns_now = SDL_GetTicksNS();
            time_ns_delta = time_ns_now - time_ns_last;
        }
    }

    // clear the buffer    
    SDL_SetRenderDrawColor(renderer, 32, 32, 32, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // draw the scene
    scene.draw(*renderer, time_ns_delta / static_cast<float>(ONE_SECOND_NS));

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    scene.draw_gui();
    // ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

    // display the render
    SDL_RenderPresent(renderer);

    frame_number++;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    ImGui_ImplSDL3_ProcessEvent(event);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(window);
}
