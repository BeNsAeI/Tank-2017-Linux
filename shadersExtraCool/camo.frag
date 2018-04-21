#version 130
uniform float uKa, uKd,uAd,uBd, uTol,uNoiseFreq,uNoiseAmp,uAlpha,uTime; // coefficients of each type of lighting
uniform float uShininess; // specular exponent
uniform sampler2D Noise2;

in vec2 vST; // texture cords
in vec3 vN; // normal vector
in vec3 vL; // vector from point to light
in vec4 vColor;
in vec3 vMCposition;

vec2 SineWave( vec2 p )
{
    float pi = 3.14159;
    float A = 0.15;
    float w = 10.0 * pi;
    float t = 30.0*pi/180.0;
    float y = sin( w*p.x + t) * A; 
    return vec2(p.x, p.y+y);
}

void
main( )
{
	vec3 camoColor = vec3(vColor.r/4,vColor.g/2,vColor.b/2);
	//vec3 camoColor = vec3(0,0,0);
	vec3 baseColor = vec3(vColor.r*3/4,vColor.g*3/2,vColor.b*3/2);
	//vec3 baseColor = vec3(1,0,0);
	
	vec3 myColor = baseColor;
	
	float Ar = uAd/2.;
	float Br = uBd/2.;
	int numins = int( vMCposition.x*vMCposition.y/40/*vST.s*/ / uAd );
	int numint = int( (vMCposition.z-2.5)*vMCposition.y/40/*vST.t*/ / uBd );
	float u_c = numins *uAd + Ar ;
	float v_c = numint *uBd + Br ;
	
	vec4 nv = texture2D( Noise2, uNoiseFreq*vMCposition.xy );
	float n = nv.r + nv.g + nv.b + nv.a;    //  1. -> 3.
	n = n - 2.;
	
	float sc = float(numins) * uAd  +  Ar;
	float ds = vMCposition.x*vMCposition.y/40/*vST.s*/ - sc;                   // wrt ellipse center
	float tc = float(numint) * uBd  +  Br;
	float dt = (vMCposition.z-2.5)*vMCposition.y/40/*vST.t*/ - tc;                   // wrt ellipse center

	float oldDist = sqrt( ds*ds + dt*dt );
	float newDist = oldDist + uNoiseAmp*n;
	float scale = newDist / oldDist;        // this could be < 1., = 1., or > 1.

	ds *= scale;                            // scale by noise factor
	ds /= Ar;                               // ellipse equation

	dt *= scale;                            // scale by noise factor
	dt /= Br;                               // ellipse equation

	float dist = ds*ds + dt*dt;
	float a=1.;
	if(dist < 1 - uTol * 2 * uTime)
	{
		if (uAlpha==0)
			discard;
		else
			a=uAlpha;
	}
	if(dist < 1 - uTol)
	{
		myColor=camoColor;
	}
	if( abs( dist - 1 ) < uTol )
	{
		float t = smoothstep( 1 - uTol, 1 + uTol, dist );
		myColor = vec3(1,mix( vec4( baseColor, 1. ), vec4( camoColor, 1. ), t ).g,0);
	}
	
	vec3 Light = normalize(vL);
	vec3 ambient = uKa * myColor;
	float d = max( dot(vN,Light), 0. ); // only do diffuse if the light can see the point
	vec3 diffuse = uKd * d * myColor;
	vec3 shaded = ambient + diffuse;
	
	
	gl_FragColor = vec4( shaded, 1 );
}