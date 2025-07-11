#version 460 core

layout(location = 0) in vec2 u_Position;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * vec4(u_Position, 0.0, 1.0f);
}