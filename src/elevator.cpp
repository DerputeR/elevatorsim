#include "elevator.h"

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
            move_dir = Direction::down;
        } else if (floor > current_floor) {
            move_dir = Direction::up;
        }
        current_floor = floor;
        return true;
    }
    return false;
}

void single_scan(Elevator& e) {

}

void single_look(Elevator& e) {

}

void single_cscan(Elevator& e) {

}

void single_clook(Elevator& e) {

}
