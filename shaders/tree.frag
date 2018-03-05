#version 130
in vec4 color;
in float height;
void
main( )
{
	vec3 myColor;

	myColor.r = (color.r + fract(sin(dot(vec2(color.r*0.8278,color.r),vec2(12.9898,78.233)))*43758.5453)*2)*height;
	myColor.g = (color.g + fract(sin(dot(vec2(color.g*0.8278,color.g),vec2(12.9898,78.233)))*43758.5453)*2)*height/2;
	myColor.b = (color.b + fract(sin(dot(vec2(color.b*0.8278,color.b),vec2(12.9898,78.233)))*43758.5453)*2)*height;

	gl_FragColor = vec4(myColor,1);
}
