#version 130
out vec3 vN; // normal vector
out vec3 vL; // vector from point to light
out float color;
vec3 LIGHTPOSITION = vec3( 20, 50, 35 );

void
main( )
{
	vec4 model = gl_Vertex;
	vec4 ECposition = gl_ModelViewMatrix * gl_Vertex;
	vN = normalize( gl_NormalMatrix * gl_Normal ); // normal vector
	vL = LIGHTPOSITION - ECposition.xyz; // vector from the point
	model.y = model.y+2*fract(sin(dot(vec2(model.x,model.z),vec2(12.9898,78.233)))*43758.5453);
	color = gl_Color.g;
	gl_Position = gl_ModelViewProjectionMatrix * model;
}
