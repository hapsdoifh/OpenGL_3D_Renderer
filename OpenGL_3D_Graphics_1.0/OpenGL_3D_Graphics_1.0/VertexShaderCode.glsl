
#version 430
	
in layout(location=0) vec3 position; //location that came in
in layout(location=1) vec3 vertexColor; //location <==> attribute


uniform mat4 fullTransformationMatrix;

out vec3 theColor;
	
void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position = fullTransformationMatrix * v;
	theColor = vertexColor;
};