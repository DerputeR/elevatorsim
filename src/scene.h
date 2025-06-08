#pragma once

#include <SDL3/SDL_render.h>

class Scene {
public:
	void draw(SDL_Renderer& renderer, float delta_time) const;
	void draw_gui();
};
