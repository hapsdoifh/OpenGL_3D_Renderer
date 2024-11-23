#version 430

out vec4 daColor; //location that came in
in vec3 theColor;

void main()
{
	daColor = vec4(theColor, 1.0);
};

