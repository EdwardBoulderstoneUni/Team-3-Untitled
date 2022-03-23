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

float snoise(vec2 v);
vec4 projectTexture();
vec4 applyLighting(vec4 albedo);

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
 albedo = mix(albedo, texture(mask, IN.texCoord), generatePaintedMap());
 fragColor = applyLighting(albedo);
}