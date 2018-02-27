#version 130
uniform float uKa, uKd,uAd,uBd, uTol,uNoiseFreq,uNoiseAmp,uAlpha,uTime; // coefficients of each type of lighting
uniform float uShininess; // specular exponent
uniform sampler3D Noise3;

in vec2 vST; // texture cords
in vec3 vN; // normal vector
in vec3 vL; // vector from point to light
in vec4 vColor;
in vec3 vMCposition;

void
main( )
{
	vec3 camoColor = vec3(vColor.r/8,vColor.g/4,vColor.b/4);
	//vec3 camoColor = vec3(0,0,0);
	vec3 baseColor = vec3(vColor.r*3/4,vColor.g*3/2,vColor.b*3/2);
	//vec3 baseColor = vec3(1,0,0);
	
	vec3 myColor = baseColor;
	
	float Ar = uAd/2.;
	float Br = uBd/2.;
	int numins = int( vST.s / uAd );
	int numint = int( vST.t / uBd );
	float u_c = numins *uAd + Ar ;
	float v_c = numint *uBd + Br ;
	
	vec4 nv = texture3D( Noise3, uNoiseFreq*vMCposition );
	float n = nv.r + nv.g + nv.b + nv.a;    //  1. -> 3.
	n = n - 2.;
	
	float sc = float(numins) * uAd  +  Ar;
	float ds = vST.s - sc;                   // wrt ellipse center
	float tc = float(numint) * uBd  +  Br;
	float dt = vST.t - tc;                   // wrt ellipse center

	float oldDist = sqrt( ds*ds + dt*dt );
	float newDist = oldDist + uNoiseAmp*n;
	float scale = newDist / oldDist;        // this could be < 1., = 1., or > 1.

	ds *= scale;                            // scale by noise factor
	ds /= Ar;                               // ellipse equation

	dt *= scale;                            // scale by noise factor
	dt /= Br;                               // ellipse equation

	float dist = ds*ds + dt*dt;
	float a=1.;
	float rand = vST.s*abs(sin(1000*uTime*1))*fract(sin(dot(vec2(vST.t*abs(sin(1000*uTime*1)),vST.s*abs(sin(1000*uTime*1))),vec2(12.9898,78.233)))*43758.5453);
	if(dist < 1 - uTol * 2 * rand)
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