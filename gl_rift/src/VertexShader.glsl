#version 330 core

// Input data
layout (location = 0) in vec3 vertex_pos_modelspace;
layout (location = 1) in vec2 vertexUV;
 
// Output data 
out vec2 UV;

uniform mat4 MVP;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position = MVP * vec4(vertex_pos_modelspace, 1);

	// Pass texture coordinates through
	UV = vertexUV;
}						
			