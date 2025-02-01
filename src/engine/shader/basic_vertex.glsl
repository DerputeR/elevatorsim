#ifdef CPP_GLSL_INCLUDE
R"(
#endif

// shader start

#version 330 core
layout (location = 0) in vec3 pos;

void main() {
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}

// shader end

#ifdef CPP_GLSL_INCLUDE
)";
#endif