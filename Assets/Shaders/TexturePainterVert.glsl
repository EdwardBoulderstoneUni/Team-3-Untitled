#version 400 core

uniform mat4 modelMatrix 	= mat4(1.0f);
uniform mat4 viewMatrix 	= mat4(1.0f);
uniform mat4 projMatrix 	= mat4(1.0f);

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;

out Vertex
{
	vec2 texCoord;
	vec3 worldPos;
} OUT;

void main(void)
{
	mat4 mvp = (projMatrix * viewMatrix * modelMatrix);
	OUT.worldPos = (modelMatrix * vec4 (position, 1)).xyz;
	OUT.texCoord = texCoord;
	gl_Position = (position.x * 2 - 1, 1 - 2 * position.y, 0 , 1);
}