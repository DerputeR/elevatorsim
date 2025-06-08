#include "scene.h"
#include <SDL3/SDL_rect.h>

void Scene::draw(SDL_Renderer& renderer, float delta_time) const {
    SDL_FRect rect;

    SDL_SetRenderDrawColor(&renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);  /* blue, full alpha */
    rect.x = rect.y = 100;
    rect.w = 440;
    rect.h = 280;
    SDL_RenderFillRect(&renderer, &rect);
}