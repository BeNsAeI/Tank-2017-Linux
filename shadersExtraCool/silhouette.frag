#version 130
in vec4 gColor;
void
main( )
{
	vec3 myColor;
	myColor.r = gColor.r*2/3;
	myColor.g = gColor.g*2/3;
	myColor.b = gColor.b*2/3;
	
	gl_FragColor = vec4( myColor, 1 );
}