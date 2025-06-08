#include "scene.h"
#include <numbers>
#include <SDL3/SDL_rect.h>

static float time = 0.0f;
static float rpm = 60.0f;
constexpr float PI = std::numbers::pi_v<float>;

void Scene::draw(SDL_Renderer& renderer, float delta_time) const {
    time += delta_time;
    SDL_FRect rect;

    SDL_SetRenderDrawColor(&renderer, 125, 200, 255, SDL_ALPHA_OPAQUE);  /* blue, full alpha */
    rect.x = 100.0f + 200.0f * cosf(time * 2.0f * PI * rpm / 60.0f);
    rect.y = 100.0f + 200.0f * sinf(time * 2.0f * PI * rpm / 60.0f);
    rect.w = 440.0f;
    rect.h = 280.0f;
    SDL_RenderFillRect(&renderer, &rect);
}