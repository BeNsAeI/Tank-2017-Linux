#version 130
in float color;
in float h;
void
main( )
{
	vec3 myColor;
	myColor.r = 0.1;
	myColor.g = (color+0.3)*7.0/8.0+(h/8.0);//*fract(sin(dot(vec2(color*0.8278,color),vec2(12.9898,78.233)))*43758.5453);
	myColor.b = 0;
	vec3 ambient = myColor;
	gl_FragColor = vec4( ambient, 1);
}
