#pragma once
#include <cstddef>
#include <array>

enum class direction {
    up,
    down
};

template <int N>
struct elevator {
    const int min_floor = 0;
    const int max_floor = N - 1;
    int current_floor = 0;
    bool stopped = true;
    direction move_dir = direction::up;
    std::array<bool, N> floors_called = {};

    bool call_floor(int floor) {
        if (floor <= max_floor && floor >= min_floor) {
            floors_called[floor] = true;
            return true;
        }
	return false;
    }

    bool move_to_floor(int floor) {
        if (floor <= max_floor && floor >= min_floor) {
            floors_called[floor] = false;
	    if (floor < current_floor) {
                move_dir = direction::down;
	    } else if (floor > current_floor) {
                move_dir = direction::up;
            }
	    current_floor = floor;
	    return true;
        }
        return false;
    }
};

void single_scan(elevator& e);
void single_look(elevator& e);
void single_cscan(elevator& e);
void single_clook(elevator& e);


