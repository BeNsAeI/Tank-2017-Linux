#version 130

in vec4 gColor;
in float height;

void
main( )
{
	vec3 myColor;
	myColor.r = gColor.r*3/2*height;
	myColor.g = gColor.g*3/2*height;
	myColor.b = gColor.b*3/2*height;
	vec3 shaded = myColor;
	gl_FragColor = vec4( shaded, 1 );
}