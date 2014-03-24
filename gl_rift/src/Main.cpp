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
// SDL
#include <SDL.h>
#include <SDL_opengl.h>
// Helper classes
#include "Shapes.h"
#include "Shaders.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << "error: " << SDL_GetError() << std::endl;
}

void logGLEWError(std::ostream &os, const std::string &msg) {
	os << "GLEW error: " << msg << std::endl;
	
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren) {
	// Initialize to null to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;
	// Load the image
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	if (loadedImage != nullptr) {
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		if (texture == nullptr) {
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

int draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
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

	startup();

	// Rendering loop
	int rendering = 1;
	while (rendering) {
		//draw();
		time_t timer;
		render(time(&timer));
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