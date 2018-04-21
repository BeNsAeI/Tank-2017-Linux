#version 130
uniform float uKa, uKd; // coefficients of each type of lighting

in vec3 vN; // normal vector
in vec3 vL; // vector from point to light
in float color;
void
main( )
{
	vec3 myColor;
	myColor.r = 0.1;
	myColor.g = color+1.5*fract(sin(dot(vec2(color*0.8278,color),vec2(12.9898,78.233)))*43758.5453);
	myColor.b = 0;
	vec3 Light = normalize(vL);
	vec3 ambient = uKa * myColor;
	float d = max( dot(vN,Light), 0. ); // only do diffuse if the light can see the point
	vec3 diffuse = uKd * d * myColor;
	gl_FragColor = vec4( ambient + diffuse/2, 1);
}
