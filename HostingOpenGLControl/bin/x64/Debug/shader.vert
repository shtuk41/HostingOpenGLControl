#version 330

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 mv;
uniform mat4 p;

smooth out vec3 theColor;
out vec2 TexCoord;

void main()
{
	gl_Position = p * mv * inPosition;
	theColor = inColor;
	TexCoord = aTexCoord;
}