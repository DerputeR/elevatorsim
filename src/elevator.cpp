#include "elevator.h"

static int find_next_floor_in_direction(const Elevator& e, const Direction dir);

Direction operator-(Direction& d) {
    switch (d) {
    case Direction::Up:
        return Direction::Down;
    case Direction::Down:
        return Direction::Up;
    }
}

bool Elevator::call_floor(int floor) {
    if (floor <= max_floor && floor >= min_floor) {
        floors_called[floor] = true;
        return true;
    }
    return false;
}

bool Elevator::move_to_floor(int floor) {
    if (floor <= max_floor && floor >= min_floor) {
        floors_called[floor] = false;
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

void single_scan(Elevator& e) {
    // check if there are any called floors in the current direction
    int next_floor = find_next_floor_in_direction(e, e.move_dir);

    // if not, try the opposite direction only if we're at the limits
    if (next_floor == -1) {
        if (e.current_floor == (e.move_dir == Direction::Down ? e.min_floor : e.max_floor)) {
            next_floor == find_next_floor_in_direction(e, -e.move_dir);
        }
    }

    // if there are no more floors to service, exit early
    if (next_floor == -1) {
        e.stopped = true;
        return;
    }
    else {
        e.next_floor = next_floor;
    }
}

static int find_next_floor_in_direction(const Elevator& e, const Direction dir) {
    int next_floor = -1;
    for (int i = e.current_floor;
            dir == Direction::Up ? i <= e.max_floor : i >= e.min_floor;
            i++) {
        if (e.floors_called[i]) {
            next_floor = e.floors_called[i];
            break;
        }
    }
    return next_floor;
}

void single_look(Elevator& e) {
    // check if there are any called floors in the current direction
    int next_floor = find_next_floor_in_direction(e, e.move_dir);

    // if not, try the opposite direction
    if (next_floor == -1) {
        next_floor == find_next_floor_in_direction(e, -e.move_dir);
    }

    // if there are no more floors to service, exit early
    if (next_floor == -1) {
        e.stopped = true;
        return;
    }
    else {
        e.next_floor = next_floor;
    }
}
