#version 130
out vec4 color;
out float height;
void
main( )
{
	height = (gl_Vertex.z+1);
	color = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
