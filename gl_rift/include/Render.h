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

int draw_stereo(GLuint programID);
int draw(GLuint programID);