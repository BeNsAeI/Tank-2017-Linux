#version 130
out vec3 vN; // normal vector
out vec3 vL; // vector from point to light
out vec4 color;
const vec3 LIGHTPOSITION = vec3( 20, 15, 35 );

void
main( )
{
	vec4 ECposition = gl_ModelViewMatrix * gl_Vertex;
	vN = normalize( gl_NormalMatrix * gl_Normal ); // normal vector
	vL = LIGHTPOSITION - ECposition.xyz; // vector from the point
	color = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
