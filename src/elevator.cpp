#include "elevator.h"

bool elevator::call_floor(int floor) {
    if (floor <= max_floor && floor >= min_floor) {
        floors_called[floor] = true;
        return true;
    }
    return false;
}

bool elevator::move_to_floor(int floor) {
    if (floor <= max_floor && floor >= min_floor)
{
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

void single_scan(elevator& e) {

}

void single_look(elevator& e) {

}

void single_cscan(elevator& e) {

}

void single_clook(elevator& e) {

}
