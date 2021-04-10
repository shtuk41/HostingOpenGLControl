#version 330

smooth in vec3 theColor;
in vec2 TexCoord;

out vec4 outputColor;

uniform sampler2D ourTexture;

void main()
{
	//outputColor = vec4(theColor, 1.0);

	outputColor = texture(ourTexture, TexCoord);



	outputColor.w = 1.0;
}