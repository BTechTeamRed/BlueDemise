#version 330 core
in vec2 uv;

out vec4 color;
uniform vec4 col;
uniform sampler2D myTextureSampler;

void main()
{
	color = texture( myTextureSampler, uv).rgba * col;
}