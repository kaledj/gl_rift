// Standard stuff
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include <minmax.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);