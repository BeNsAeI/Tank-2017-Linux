#version 130
uniform float uKa, uKd, uKs; // coefficients of each type of lighting
uniform float uShininess; // specular exponent
uniform sampler2D ImageUnit;

in vec3 gN; // normal vector
in vec3 gL; // vector from point to light
in vec4 gColor;


void
main( )
{
	vec3 myColor;
	myColor.r = gColor.r*3/2;
	myColor.g = gColor.g*3/2;
	myColor.b = gColor.b*3/2;

	vec3 Light = normalize(gL);
	vec3 ambient = uKa * myColor;
	float d = max( dot(gN,Light), 0. ); // only do diffuse if the light can see the point
	vec3 diffuse = uKd * d * myColor;
	vec3 shaded = ambient + diffuse;
	
	//Toon shading:
	float uMagTol=1;
	float uQuantize=200;
	// Store the buffer to a texture unit
	
	
	gl_FragColor = vec4( shaded, 1 );
}