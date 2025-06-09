#include "elevator.h"

static int find_next_floor_in_direction(const Elevator& e, const Direction dir);

Direction operator-(Direction& d) {
    switch (d) {
    case Direction::Up:
        return Direction::Down;
    case Direction::Down:
        return Direction::Up;
    default:
        return d;
    }
}

static int find_next_floor_in_direction(const Elevator& e, const Direction dir) {
    int next_floor = -1;
    for (int i = e.current_floor;
        dir == Direction::Up ? i <= e.max_floor : i >= e.min_floor;
        dir == Direction::Up ? i++ : i--) {
        if (e.is_floor_called(i)) {
            next_floor = i;
            break;
        }
    }
    return next_floor;
}

bool Elevator::call_floor(int floor, bool state) {
    if (floor <= max_floor && floor >= min_floor) {
        floors_called[floor - min_floor] = state;
        return true;
    }
    return false;
}

bool Elevator::is_floor_called(int floor) const {
    if (floor <= max_floor && floor >= min_floor) {
        return floors_called[floor - min_floor];
    }
    return false;
}

bool Elevator::is_stopped() const {
    return stop_timer > 0.0f;
}

void Elevator::set_stopped(bool state) {
    if (state) {
        stop_timer = stop_duration;
    }
    else {
        stop_timer = 0.0f;
    }
}

bool Elevator::move_to_floor(int floor) {
    if (floor <= max_floor && floor >= min_floor) {
        floors_called[floor - min_floor] = false;
        if (floor < current_floor) {
            move_dir = Direction::Down;
        } else if (floor > current_floor) {
            move_dir = Direction::Up;
        }
        current_floor = floor;
        return true;
    }
    return false;
}

void Elevator::single_scan(Elevator& e) {
    // check if there are any called floors in the current direction
    int next_floor = find_next_floor_in_direction(e, e.move_dir);

    // if not, try the opposite direction only if we're at the limits
    if (next_floor == -1) {
        if (e.current_floor == (e.move_dir == Direction::Down ? e.min_floor : e.max_floor)) {
            next_floor = find_next_floor_in_direction(e, -e.move_dir);
        }
    }

    // if there are no more floors to service, exit early
    if (next_floor == -1) {
        return;
    }
    else {
        e.next_floor = next_floor;
    }
}

void Elevator::single_look(Elevator& e) {
    // check if there are any called floors in the current direction
    int next_floor = find_next_floor_in_direction(e, e.move_dir);

    // if not, try the opposite direction
    if (next_floor == -1) {
        next_floor = find_next_floor_in_direction(e, -e.move_dir);
    }

    // if there are no more floors to service, exit early
    if (next_floor == -1) {
        return;
    }
    else {
        e.next_floor = next_floor;
    }
}
