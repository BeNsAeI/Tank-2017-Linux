#version 130
uniform float uX,uY,uZ;

out vec3 gN; // normal vector
out vec3 gL; // vector from point to light
out vec4 gColor;

void
main( )
{
	vec3 LIGHTPOSITION = vec3( uX, uY, uZ );
	vec4 ECposition = gl_ModelViewMatrix * gl_Vertex;
	gN = normalize( gl_NormalMatrix * gl_Normal ); // normal vector
	gL = LIGHTPOSITION - ECposition.xyz; // vector from the point
	// to the light position
	// to the eye position
	gColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}