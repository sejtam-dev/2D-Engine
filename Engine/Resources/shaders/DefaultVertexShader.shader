#version 330 core

layout(location = 0) in vec4 u_position;

void main()
{
	gl_Position = u_position;
}