#pragma once
#include <cstddef>
#include <vector>

enum class Direction {
    Up,
    Down
};

Direction operator-(Direction& d);

struct Elevator {
    const int min_floor = 1;
    const int max_floor;
    int current_floor = min_floor;
    int next_floor = min_floor;
    bool stopped = true;
    Direction move_dir = Direction::Up;
    std::vector<bool> floors_called;

    // ctor
    Elevator(int N) : max_floor{N}, floors_called(N, false) { }

    bool call_floor(int floor);
    bool is_floor_called(int floor) const;
    bool move_to_floor(int floor);
};

void single_scan(Elevator& e);
void single_look(Elevator& e);

// if this were a disk i might do these, but it's an elevator!
//void single_cscan(Elevator& e);
//void single_clook(Elevator& e);


