#version 330 core

layout (location = 0) in vec3 pos;
layout(location = 1) in vec2 vertexUV;

out vec2 uv;

uniform mat4 mvp;

uniform mat3 uvMatrix

void main()
{
	gl_Position = mvp * vec4(pos, 1);
	vec3 uvTransformed = uvMatrix * vec3(vertexUV, 1);
	uv = vec2(uvTransformed.x, uvTransformed.y);
}