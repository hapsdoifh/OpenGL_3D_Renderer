
#version 430
	
in layout(location=0) vec3 position; //location that came in
in layout(location=1) vec3 vertexColor; //location <==> attribute
//in layout(location=2) mat4 fullTransformationMatrix;
in layout(location=2) vec2 textureIn;

uniform mat4 fullTransformationMatrix;

out vec3 theColor;
out vec2 texCoord;
	
void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position = fullTransformationMatrix * v;
	theColor = vertexColor;
	texCoord = textureIn;
};