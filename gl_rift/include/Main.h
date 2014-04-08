#pragma once

// Standard stuff
#include <iostream>
#include <string>
#include <math.h>
#include <time.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
// SDL
#include <SDL.h>
#include <SDL_opengl.h>
// Helper classes
#include "Shapes.h"
#include "Shaders.h"
#include "Loader.h"
#include "Render.h"

void logSDLError(std::ostream &outstream, const std::string &message);
void logGLEWError(std::ostream &outstream, const std::string &message);
int main(int argc, char **argv);
