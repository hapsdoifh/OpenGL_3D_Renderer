#version 430

in vec3 theColor;
in vec2 texCoord;

uniform sampler2D tex0;

out vec4 daColor; //location that came in

void main()
{
	//daColor = vec4(theColor, 1.0);
	daColor = texture(tex0, texCoord);
};

