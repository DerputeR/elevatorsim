cmake_minimum_required(VERSION 3.15...3.31)

project(ElevatorSim VERSION 0.1.0
                    DESCRIPTION "Simulates a single-carriage elevator system"
                    LANGUAGES CXX)

add_executable(ElevatorSim src/elevator.cpp)