#version 400 core

uniform sampler2D mainTex;
uniform vec3 cameraPos;
uniform vec3 paintCenter;
uniform float radius;
uniform float hardness;
uniform float strength;
uniform vec4 paintColour;
in Vertex
{
	vec2 texCoord;
	vec3 worldPos;
} IN;

out vec4 fragColor;

float mask(vec3 position, vec3 center, float radius, float hardness){
	float m = distance(center, position);
	return 1 - smoothstep(radius * hardness, radius, m);    
}

void main()
{
	vec4 albedo = texture(mainTex, IN.texCoord);
	float f = mask(IN.worldPos, paintCenter, radius, hardness);
	float edge = f * strength;
	fragColor = mix(albedo, paintColour, f);
}