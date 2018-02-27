#version 130
uniform float uX,uY,uZ;

out vec2 vST; // texture coords
out vec3 vN; // normal vector
out vec3 vL; // vector from point to light
out vec3 vE; // vector from point to eye
out vec3 vMCposition;
out vec4 vColor;

void
main( )
{
	vST = gl_MultiTexCoord0.st;
	vec3 LIGHTPOSITION = vec3( uX, uY, uZ );
	vec4 ECposition = gl_ModelViewMatrix * gl_Vertex;
	vN = normalize( gl_NormalMatrix * gl_Normal ); // normal vector
	vL = LIGHTPOSITION - ECposition.xyz; // vector from the point
	// to the light position
	vE = vec3( 0., 0., 0. ) - ECposition.xyz; // vector from the point
	// to the eye position
	vColor = gl_Color;
	vMCposition  = gl_Vertex.xyz;
	gl_Position = gl_ModelViewProjectionMatrix * vec4( vMCposition, 1. );
}