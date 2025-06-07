#pragma once
#include <cstddef>
#include <vector>

enum class direction {
    up,
    down
};

struct elevator {
    const int min_floor = 0;
    const int max_floor;
    int current_floor = 0;
    bool stopped = true;
    direction move_dir = direction::up;
    std::vector<bool> floors_called;

    elevator(int N) : max_floor{N - 1}, floors_called(N, false) { }

    bool call_floor(int floor);
    bool move_to_floor(int floor);
};

void single_scan(elevator& e);
void single_look(elevator& e);
void single_cscan(elevator& e);
void single_clook(elevator& e);


