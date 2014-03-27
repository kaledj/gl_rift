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
#include "loadShaders.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

GLuint programID;

void logSDLError(std::ostream &os, const std::string &msg) 
{
	os << msg << "error: " << SDL_GetError() << std::endl;
}

void logGLEWError(std::ostream &os, const std::string &msg) 
{
	os << "GLEW error: " << msg << std::endl;
	
}

int draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Vertex Array Object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_buffer_data), 
		cube_vertex_buffer_data, GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(0);
	return 0;
}

int main(int arc, char **argv) {	
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	// Create a window
	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, 
		SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (win == nullptr) {
		logSDLError(std::cout, "SDL_CreateWindow");
		return 2;
	}

	// Initialize and set up OpenGL
	SDL_GLContext sdl_opengl_context = SDL_GL_CreateContext(win);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetSwapInterval(1);

	// Initialize GLEW
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		logGLEWError(std::cout, "glewInit");
		return 3;
	}

	programID = LoadShaders("../gl_rift/src/VertexShader.glsl", "../gl_rift/src/FragmentShader.glsl");
	glUseProgram(programID);
	//startup();
			
	// Rendering loop
	int rendering = 1;
	while (rendering) {
		draw();
		//render(SDL_GetTicks() /100);
		SDL_GL_SwapWindow(win);
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				rendering = 0;
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					rendering = 0;
				}
				break;
			}
		}
	}
	shutdown();
	SDL_GL_DeleteContext(sdl_opengl_context);
	SDL_DestroyWindow(win);

	SDL_Quit();
	return 0;	
}