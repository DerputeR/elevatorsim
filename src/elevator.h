#pragma once
#include <cstddef>
#include <vector>

enum class Direction {
    Up,
    Down
};

Direction operator-(Direction& d);

struct Elevator {
    // elevator dimensions
    const int min_floor = 1;
    const int max_floor;

    // things the algorithm needs to work
    int current_floor = min_floor;
    int next_floor = min_floor;
    Direction move_dir = Direction::Up;
    std::vector<bool> floors_called;

    // other internals for the purposes of a nice simulation
    float stop_timer = 0.0f;
    float stop_duration = 2.0f;
    float y_position = 1.0f; // actual position of the elevator car
    float move_speed = 1.0f;

    // ctor
    Elevator(int N) : max_floor{N}, floors_called(N, false) { }

    // helper functions
    bool call_floor(int floor, bool state = true);
    bool is_floor_called(int floor) const;
    bool move_to_floor(int floor);
    bool is_stopped() const;
    void set_stopped(bool state);

    static void single_scan(Elevator& e);
    static void single_look(Elevator& e);

    // if this were a disk i might do these, but it's an elevator!
    //static void single_cscan(Elevator& e);
    //static void single_clook(Elevator& e);
};


