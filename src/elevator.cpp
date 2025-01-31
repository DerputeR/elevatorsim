#include "elevator.h"
#include "app_info.h"
#include <iostream>
#include <array>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include "engine/shader_loader.h"

static SDL_Window* window;
static SDL_GLContext context; // this is actually a pointer

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
    SDL_SetAppMetadata("Elevator Simulation", APP_VERSION, "com.example.elevatorsim");
    SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ASSERT, "SDL failed to init: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Window init
    window = SDL_CreateWindow("Elevator Simulation", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Failed to create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // OpenGL context init
    context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Failed to create OpenGL context for window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    if (vsync_enabled) {
        if (vsync_adaptive) {
            if (!SDL_GL_SetSwapInterval(-1)) {
                SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to enable adaptive vsync: %s", SDL_GetError());
                if (!SDL_GL_SetSwapInterval(1)) {
                    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to enable vsync: %s", SDL_GetError());
                }
            }
        } else if (!SDL_GL_SetSwapInterval(1)) {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to enable vsync: %s", SDL_GetError());
        }
    } else if (!SDL_GL_SetSwapInterval(0)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to disable vsync: %s", SDL_GetError());
    }

    // GLAD init
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Failed to initialize GLAD");
        return SDL_APP_FAILURE;
    }
    
    // Dear ImGUI init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, &context);
    ImGui_ImplOpenGL3_Init();

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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // display the render
    SDL_GL_SwapWindow(window);

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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(window);
}