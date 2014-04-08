#include "Main.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;
const bool FULLSCREEN = true;

void logSDLError(std::ostream &os, const std::string &msg) 
{
	os << msg << "error: " << SDL_GetError() << std::endl;
}

void logGLEWError(std::ostream &os, const std::string &msg) 
{
	os << "GLEW error: " << msg << std::endl;
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
	if (FULLSCREEN) {
		SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	// Initialize and set up OpenGL
	SDL_GLContext sdl_opengl_context = SDL_GL_CreateContext(win);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetSwapInterval(1);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		logGLEWError(std::cout, "glewInit");
		return 3;
	}

	GLuint programID = LoadShaders("../gl_rift/src/VertexShader.glsl", "../gl_rift/src/FragmentShader.glsl");
	glUseProgram(programID);
	//startup();
	
	// Rendering loop
	int rendering = 1;
	while (rendering) {
		draw_stereo(programID);
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
	glDeleteProgram(programID);
	SDL_GL_DeleteContext(sdl_opengl_context);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;	
}