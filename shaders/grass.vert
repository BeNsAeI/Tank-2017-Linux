#version 130
out float color;

void
main( )
{
	vec4 model = gl_Vertex;
	model.y = model.y+2*fract(sin(dot(vec2(model.x,model.z),vec2(12.9898,78.233)))*43758.5453);
	color = gl_Color.g;
	gl_Position = gl_ModelViewProjectionMatrix * model;
}
