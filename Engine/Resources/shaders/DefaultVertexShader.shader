#version 330 core

layout(location = 0) in vec3 u_position;

uniform mat4 camMatrix;

void main()
{
	gl_Position = camMatrix * vec4(u_position, 1.0f);
}