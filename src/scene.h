#pragma once

#include <SDL3/SDL_render.h>
#include "elevator.h"

class Scene {
	float delta_time = 0.0f;
	float total_time = 0.0f;
	Elevator elevator = Elevator(8);
public:
	void update(float delta_time);
	void draw(SDL_Renderer& renderer) const;
	void draw_gui();
};
