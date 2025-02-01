cmake_minimum_required(VERSION 3.21...3.31)

project(ElevatorSimProject  VERSION 0.1.0
                            DESCRIPTION "Simulates a single-carriage elevator system"
                            LANGUAGES C CXX)

configure_file("src/app_info.h.in" "${CMAKE_SOURCE_DIR}/src/app_info.h" @ONLY)

# Dependency management, excluding SDL3 (special case handled below)
include(FetchContent)
FetchContent_Declare(
    imgui            
    GIT_REPOSITORY  https://github.com/ocornut/imgui
    GIT_TAG         v1.91.7-docking
)
FetchContent_Declare(
    glm            
    GIT_REPOSITORY  https://github.com/g-truc/glm
    GIT_TAG         1.0.1
)

FetchContent_MakeAvailable(imgui glm)

# Dear ImGUI doesn't have its own CMakeLists.txt so we'll have to declare
# which sources we're using so we can embed it directly into the executable
set(IMGUI_SOURCES "${imgui_SOURCE_DIR}/imgui.cpp"
                  "${imgui_SOURCE_DIR}/imgui_demo.cpp"
                  "${imgui_SOURCE_DIR}/imgui_draw.cpp"
                  "${imgui_SOURCE_DIR}/imgui_tables.cpp"
                  "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
)

# We will also need to do so for the backend sources
set(IMGUI_BACKEND_SOURCES "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
                          "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)


# * Linking SDL instructions are found in SDL/docs/README-cmake.md

# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib/${PROJECT_NAME}")

# Create an option to switch between a system sdl library and a vendored SDL library
# If this variable exists in the cache, its value will not be overwritten
option(MYGAME_VENDORED "Use vendored libraries" OFF)
set(CMAKE_PREFIX_PATH "/usr/local" CACHE PATH "Custom prefix paths to search for libraries under")

# GLFW sets the variable BUILD_SHARED_LIBS to false before we hit SDL,
# so we need to manually enable SDL_SHARED before we aquire SDL
option(SDL_SHARED "Build a SDL shared library (if available)" ON)
option(SDL_STATIC "Build a SDL static library (if available)" OFF)

if(MYGAME_VENDORED)
    add_subdirectory("vendored/SDL" EXCLUDE_FROM_ALL)
else()
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL
        GIT_TAG         release-3.2.0
    )
    FetchContent_MakeAvailable(SDL3)
endif()

# Engine sources
set(ENGINE_SOURCES "src/engine/shader_loader.cpp" "src/engine/engine.cpp")

# Create the executable target.
# Since Dear ImGUI is not being built as a separate static library,
# we'll need to include the sources here.
add_executable(ElevatorSim "src/elevator.cpp"
                           "glad/src/glad.c"
                           ${IMGUI_SOURCES}
                           ${IMGUI_BACKEND_SOURCES}
                           ${ENGINE_SOURCES}
)

# For debugging
if (MSVC AND WIN32 AND NOT MSVC_VERSION VERSION_LESS 142)
    target_link_options(ElevatorSim PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
    target_compile_options(ElevatorSim PRIVATE $<$<CONFIG:Debug>:/ZI>)
endif()

# Link to the actual SDL3 library.
target_link_libraries(ElevatorSim PRIVATE SDL3::SDL3)
target_compile_features(ElevatorSim PUBLIC cxx_std_17)
set_target_properties(ElevatorSim PROPERTIES CXX_EXTENSIONS OFF)

# Ensure the library is installed with the executable
if (TARGET SDL3-shared)
    install(TARGETS SDL3-shared
            RUNTIME DESTINATION 
            LIBRARY DESTINATION "lib"
            ARCHIVE DESTINATION "lib/${PROJECT_NAME}")
endif()


# Pick up the includes for our dependencies
target_include_directories(ElevatorSim PRIVATE "${sdl3_SOURCE_DIR}/include")
target_include_directories(ElevatorSim PRIVATE "${imgui_SOURCE_DIR}")
target_include_directories(ElevatorSim PRIVATE "glm")
target_include_directories(ElevatorSim PRIVATE "glad/include")

# Link to the library (GLM becomes a static library in this case)
target_link_libraries(ElevatorSim PRIVATE glm)

# Ensure the executable is also placed in bin
install(TARGETS ElevatorSim RUNTIME DESTINATION "bin")
