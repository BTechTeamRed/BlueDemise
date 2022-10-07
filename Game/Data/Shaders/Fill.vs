#version 330 core

layout (location = 0) in vec3 pos;
layout(location = 1) in vec2 vertexUV;
//layout (location = 2) in vec2 aTexCoord;

out vec2 uv;
//out vec2 TexCoord;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(pos, 1);
	uv = vertexUV;
	//TextCoord = aTexCoord;
}