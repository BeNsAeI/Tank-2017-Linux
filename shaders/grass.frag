#version 130
in float color;
void
main( )
{
	vec3 myColor;
	myColor.r = 0.1;
	myColor.g = color+1.5*fract(sin(dot(vec2(color*0.8278,color),vec2(12.9898,78.233)))*43758.5453);
	myColor.b = 0;
	vec3 ambient = myColor;
	gl_FragColor = vec4( ambient, 1);
}
