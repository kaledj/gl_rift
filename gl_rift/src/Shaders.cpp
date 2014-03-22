// Standard stuff
#include <iostream>
#include <string>
#include <math.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "Shaders.h"

GLuint rendering_program;
GLuint vertex_array_object;

void render(double currentTime)
{
	const GLfloat color[] = { 0.0f, 0.2, 0.0f, 1.0f };
	const GLfloat tri_color[] = { 0.0f, 0.8, 0.3f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);

	// Use the program we created for rendering 
	glUseProgram(rendering_program);

	GLfloat attrib[] = { (float)sin(currentTime) * 0.5f + 0.5f,
		(float)cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f };

	// Assign attributes to go into vertex shader
	glVertexAttrib4fv(0, attrib);
	glVertexAttrib4fv(1, tri_color);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPointSize(5);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}



void shutdown()
{
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteProgram(rendering_program);
	glDeleteVertexArrays(1, &vertex_array_object);
}

GLuint compile_shaders(void)
{
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint tessellation_control_shader;
	GLuint tessellation_evaluation_shader;
	GLuint geometry_shader;
	GLuint program;

	static const GLchar* vertex_shader_source[] =
	{
		"#version 430 core													\n"
		"// offset and color are inputs to vertex shader					\n"
		"layout (location = 0) in vec4 offset;								\n"
		"layout (location = 1) in vec4 color;								\n"
		"out VS_OUT															\n"
		"{																	\n"
		"	vec4 color;														\n"
		"} vs_out;															\n"
		"void main(void)													\n"
		"{																	\n"
		"	const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),	\n"
		"									 vec4(-0.25, -.25, 0.5, 1.0),	\n"
		"									 vec4(0.25, 0.25, 0.5, 1.0));	\n"
		"	gl_Position = vertices[gl_VertexID] + offset;					\n"
		"   vs_out.color = color;												\n"
		"}																	\n"
	};

	static const GLchar* fragment_shader_source[] =
	{
		"#version 430 core						\n"
		"in VS_OUT								\n"
		"{										\n"
		"    vec4 color;						\n"
		"} fs_in;								\n"
		"out vec4 color;						\n"
		"void main(void)						\n"
		"{										\n"
		"	color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,				\n"
		"                cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,                \n"
		"                sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15), \n"
		"                1.0);"
		"}										\n"
	};

	static const GLchar* tessellation_control_shader_source[] =
	{
		"#version 430 core																\n"
		"layout (vertices = 3) out;														\n"
		"void main(void)																\n"
		"{																				\n"
		"	if (gl_InvocationID == 0)													\n"
		"	{																			\n"
		"		gl_TessLevelInner[0] = 5.0;												\n"
		"		gl_TessLevelOuter[0] = 5.0;												\n"
		"		gl_TessLevelOuter[1] = 5.0;												\n"
		"		gl_TessLevelOuter[2] = 5.0;												\n"
		"	}																			\n"
		"	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;	\n"
		"}"
	};

	static const GLchar* tessellation_evaluation_shader_source[] =
	{
		"#version 430 core											\n"
		"layout (triangles, equal_spacing, cw) in;					\n"
		"void main(void)											\n"
		"{															\n"
		"    gl_Position =  gl_TessCoord.x * gl_in[0].gl_Position;  \n"
		"	 gl_Position += gl_TessCoord.y * gl_in[1].gl_Position;  \n"
		"	 gl_Position += gl_TessCoord.z * gl_in[2].gl_Position;  \n"
		"} "
	};

	static const GLchar* geometry_shader_source[] =
	{
		"#version 430 core											\n"
		"layout (triangles) in;										\n"
		"layout (points, max_vertices = 3) out;						\n"
		"void main(void)											\n"
		"{															\n"
		"    int i;													\n"
		"    for(i = 0; i < gl_in.length(); i++) {					\n"
		"        gl_Position = gl_in[i].gl_Position;				\n"
		"        EmitVertex();                                      \n"
		"     }                                                     \n"
		"} "
	};

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	tessellation_control_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tessellation_control_shader, 1, tessellation_control_shader_source, NULL);
	glCompileShader(tessellation_control_shader);

	tessellation_evaluation_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tessellation_evaluation_shader, 1, tessellation_evaluation_shader_source, NULL);
	glCompileShader(tessellation_evaluation_shader);

	geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry_shader, 1, geometry_shader_source, NULL);
	glCompileShader(geometry_shader);


	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	//glAttachShader(program, tessellation_control_shader);
	//glAttachShader(program, tessellation_evaluation_shader);
	//glAttachShader(program, geometry_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(tessellation_control_shader);
	glDeleteShader(tessellation_evaluation_shader);
	glDeleteShader(geometry_shader);

	return program;
}

void startup()
{
	rendering_program = compile_shaders();
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
}
