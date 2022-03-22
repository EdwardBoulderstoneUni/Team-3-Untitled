#version 400 core

uniform vec4 		objectColour;
uniform sampler2D 	mainTex;
uniform sampler2DShadow shadowTex;

uniform vec3	lightPos;
uniform float	lightRadius;
uniform vec4	lightColour;

uniform vec3	cameraPos;

uniform bool hasTexture;

in Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColor;

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

void main()
{
 vec4 albedo = projectTexture();
 fragColor = applyLighting(albedo);
}