#version 330 core

// Interpolated texture coordinates from vertex shader
in vec2 UV;

// Output data 
out vec3 color;
 
uniform sampler2D texture_sampler;

void main()
{
	color = vec3(1, 0, 0);
    //color = texture(texture_sampler, UV).rgb;
}
