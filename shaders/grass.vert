#version 130
out float color;
out float h;
void
main( )
{
	vec4 model = gl_Vertex;
	model.y = model.y+2.0*fract(sin(dot(vec2(model.x,model.z),vec2(12.9898,78.233)))*43758.5453);
	h = model.y;
	color = abs(sin(model.y*fract(sin(dot(vec2(model.x,model.z),vec2(12.9898,78.233)))*43758.5453)))/4;
	gl_Position = gl_ModelViewProjectionMatrix * model;
}
