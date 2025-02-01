#include "engine.h"
#include <string>
#include "shader_loader.h"
#include <SDL3/SDL_log.h>
#define CPP_GLSL_INCLUDE

Engine::Engine() {
    glGenBuffers(1, &this->vbo);
}

Engine::~Engine() {
    glDeleteBuffers(1, &this->vbo);
}

void Engine::add_triangles(const std::vector<Vertex>& vertices) {
    size_t size = vertices.size();
    if (size % 3 != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to add triangles; buffer contains %llu vertices", size);
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
}

void Engine::load_default_shaders() {
    GLchar* vertex_src = 
#include "shader/basic_vertex.glsl"
    ;

    GLchar* frag_src =
#include "shader/basic_frag.glsl"
    ;

    ShaderLoader::create_program(vertex_src, frag_src);

}