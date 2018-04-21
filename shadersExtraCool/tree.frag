#version 130
uniform float uKa, uKd; // coefficients of each type of lighting

in vec3 vN; // normal vector
in vec3 vL; // vector from point to light
in vec4 color;
void
main( )
{
	vec3 myColor;

	myColor.r = (color.r + fract(sin(dot(vec2(color.r*0.8278,color.r),vec2(12.9898,78.233)))*43758.5453)*2);
	myColor.g = (color.g + fract(sin(dot(vec2(color.g*0.8278,color.g),vec2(12.9898,78.233)))*43758.5453)*2)/2;
	myColor.b = (color.b + fract(sin(dot(vec2(color.b*0.8278,color.b),vec2(12.9898,78.233)))*43758.5453)*2)/2;

	vec3 Light = normalize(vL);
	vec3 ambient = uKa * myColor;
	float d = max( dot(vN,Light), 0. ); // only do diffuse if the light can see the point
	vec3 diffuse = uKd * d * myColor;
	gl_FragColor = vec4( ambient + diffuse, 1);
}
