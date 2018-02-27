#version 130
out vec4 gColor;

void
main( )
{
	gColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}