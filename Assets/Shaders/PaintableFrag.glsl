#version 400 core

uniform vec4 		objectColour;
uniform sampler2D 	mainTex;
uniform sampler2DShadow shadowTex;

uniform vec3	lightPos;
uniform float	lightRadius;
uniform vec4	lightColour;

uniform vec3	cameraPos;
uniform bool hasTexture;

uniform sampler2D mask;

in Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColor;


float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

vec4 projectTexture(){
	vec4 albedo = IN.colour;
	if(hasTexture) {
		albedo *= texture(mainTex, IN.texCoord);
	}
	return albedo;
}

vec4 applyLighting(vec4 albedo){
	float shadow = 1.0;
	if( IN.shadowProj.w > 0.0) { 
		shadow = textureProj(shadowTex, IN.shadowProj) * 0.5f;
	}
	vec3  incident = normalize (lightPos - IN.worldPos );
	float lambert  = max (0.0 , dot (incident, IN.normal)) * 0.9; 
	vec3 viewDir = normalize (cameraPos - IN.worldPos);
	vec3 halfDir = normalize (incident + viewDir);
	float rFactor = max (0.0 , dot ( halfDir, IN.normal ));
	float sFactor = pow ( rFactor , 80.0 );
	
    vec4 outputColour;
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	outputColour.rgb = albedo.rgb * 0.05f; //ambient
	outputColour.rgb += albedo.rgb * lightColour.rgb * lambert * shadow; //diffuse light
	outputColour.rgb += lightColour.rgb * sFactor * shadow; //specular light
	outputColour.rgb = pow(outputColour.rgb, vec3(1.0 / 2.2f));
	outputColour.a = albedo.a;

    return outputColour;
}

float paintedFlag(){
	float noiseScale = 50.0;
	float noise = snoise(IN.texCoord * noiseScale);

	float paintChannel = texture(mask, IN.texCoord).a;
	float paintBlur = (noise *  paintChannel) + paintChannel;

	float isPainted = 0.0;
	if (1-paintChannel < paintBlur){
		isPainted = 1.0;
	}

	return isPainted;
}

void main()
{
 vec4 albedo = projectTexture();
 albedo = mix(albedo, texture(mask, IN.texCoord), paintedFlag());
 fragColor = applyLighting(albedo);
}