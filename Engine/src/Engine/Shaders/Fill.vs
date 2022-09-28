#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 offset;
layout (location = 2) in vec2 size;

uniform mat4 projection;
//uniform mat4 mvm;
//uniform mat4 mvp;

void main()
{
	gl_Position = projection * vec4((pos * size) + offset, 0.0, 1.0);
	//gl_Position = pos * mvp;
}