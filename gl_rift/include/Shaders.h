#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

void render(double currentTime);
void shutdown();
GLuint compile_shaders(void);
void startup();