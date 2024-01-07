#version 460 core

layout(location = 0) in vec3 u_position;

void main()
{
	gl_Position = vec4(u_position, 1.0f);
}