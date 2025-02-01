#ifdef CPP_GLSL_INCLUDE
R"(
#endif

// shader start

#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 

// shader end

#ifdef CPP_GLSL_INCLUDE
)";
#endif