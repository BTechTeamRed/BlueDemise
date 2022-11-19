#version 330 core

out vec4 color;
in vec3 endColor;

void main()
{
	color = vec4(endColor, 1);
}