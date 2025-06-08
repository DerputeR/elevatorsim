#pragma once

#include <SDL3/SDL_render.h>
#include "elevator.h"

class Scene {
	Elevator elevator = Elevator(8);
public:
	void draw(SDL_Renderer& renderer, float delta_time) const;
	void draw_gui();
};
