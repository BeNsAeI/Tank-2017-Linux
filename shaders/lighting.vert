#version 130

out vec4 gColor;
out float height;
void
main( )
{
	height = (gl_Vertex.z+1)/2;
	gColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}