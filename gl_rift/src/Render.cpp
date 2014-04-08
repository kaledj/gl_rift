#include "Render.h"

int draw_stereo(GLuint programID)
{
	int HResolution = 1280;
	float InterpupillaryDistance = 0.064f;
	float fov;
	float aspectRatio;
	float zNear, zFar;

	// Set up the pipeline
	//--------------------

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Clear frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Vertex Array Object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Generate buffers
	GLuint vertexbuffer;
	GLuint tex_coords_buffer;
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &tex_coords_buffer);

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_buffer_data),
		cube_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Render
	int HRES, VRES;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &HRES, &VRES);
	fov = glm::degrees(2 * glm::atan(0.09356f, 0.082f));
	aspectRatio = (float)HRES / VRES;

	// Render the left eye 
	glViewport(0, 0, HRES / 2, VRES);

	glm::mat4 Projection = glm::perspective(fov, aspectRatio, -1.0f, 1.0f);
	float h = (0.14976f / 4) - (0.0635 / 2);
	glm::mat4 HShift = { { 1, 0, 0, h }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
	Projection = HShift * Projection;

	float v = InterpupillaryDistance / 2;
	glm::mat4 ViewShift = { { 1, 0, 0, v }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
	glm::mat4 View = glm::lookAt(
		glm::vec3(8, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model!
	glm::mat4 MVP = Projection * View * Model;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Render the right eye 
	glViewport(HRES / 2, 0, HRES / 2, VRES);

	Projection = glm::perspective(fov, aspectRatio, -1.0f, 1.0f);
	HShift = { { 1, 0, 0, -h }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
	Projection = HShift * Projection;

	ViewShift = { { 1, 0, 0, -v }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
	View = glm::lookAt(
		glm::vec3(8, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);


	Model = glm::mat4(1.0f);  // Changes for each model!
	MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);
	return 0;
}

int draw(GLuint programID)
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Clear frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat color[] = { 0, (float)117 / 255, (float)64 / 255, 1 };
	glClearBufferfv(GL_COLOR, 0, color);
	// Vertex Array Object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Generate buffers
	GLuint vertexbuffer;
	GLuint tex_coords_buffer;
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &tex_coords_buffer);

	// Bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, tex_coords_buffer);


	// Give data to buffers
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_buffer_data),
		cube_vertex_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(texture_uv_buffer_data),
	//texture_uv_buffer_data, GL_STATIC_DRAW);

	// 1st attribute buffer : vertex positions
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

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(0);
	return 0;
}