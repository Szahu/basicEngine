#type vertex
#version 420

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_color;

#define MAX_NR_SPOT_LIGHTS 5 
#define MAX_NR_POINT_LIGHTS 5 

struct PointLight
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct SpotLight
{
	vec4 Position;
	vec4 Direction;
	float InnerCutOff;
	float OuterCutOff;
	vec2 PADDING;
	vec4 Ambient;
	vec4 Diffuse;
	vec4 Specular;
	float Constant;
	float Linear;
	float Quadratic;
	float PADDING1;
};

layout (std140, binding = 1) uniform u_Lights
{
	vec2 AmountOfLights; // .x are PointLights, .y are SpotLights
	PointLight[MAX_NR_POINT_LIGHTS] u_PointLights;
	SpotLight[MAX_NR_SPOT_LIGHTS] u_SpotLights;
};


layout (std140, binding = 0) uniform u_Data
{
	mat4 u_ViewProjectionMatrix;
	vec3 u_CameraPosition;
};

uniform mat4 u_LightSpaceMatrix;

out vec3 pass_normal;
out vec3 LightDirection;
out vec4 FragPosLightSpace;
flat out vec4 pass_colour;//The "flat" qualifier stops the colour from being interpolated over the triangles.

vec3 calculateLighting();

void main(void){

	gl_Position = u_ViewProjectionMatrix * vec4(in_position, 1.0);
	
	vec3 FragPos = vec3(vec4(in_position, 1.0));
	FragPosLightSpace = u_LightSpaceMatrix * vec4(FragPos, 1.0);
	LightDirection = vec3(u_SpotLights[0].Direction);

	vec3 lighting = calculateLighting();
	pass_colour = vec4(in_color * lighting, 1.0);
	pass_normal = in_normal;
}

//simple diffuse lighting
vec3 calculateLighting(){
	
	vec3 lightDirection = vec3(u_SpotLights[0].Direction);
	vec3 lightColour = vec3(u_SpotLights[0].Diffuse);
	vec2 lightBias = vec2(0.3f, 0.8f);
	float brightness = max(dot(-lightDirection, in_normal), 0.0);
	return (lightColour * lightBias.x) + (brightness * lightColour * lightBias.y);
}

#type fragment
#version 420

flat in vec4 pass_colour;
in vec3 pass_normal;

in vec3 LightDirection;
in vec4 FragPosLightSpace;

out vec4 out_colour;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 Normal);

uniform sampler2D shadowMap;

void main(void){

	float shadow = ShadowCalculation(FragPosLightSpace, LightDirection, pass_normal);
	out_colour = pass_colour * vec4(vec3(1.0f - shadow * 0.6f), 1.0f);

}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 Normal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow

	float bias = max(0.005 * (1.0 - dot(Normal, lightDir)), 0.0005);


	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }

    shadow /= 9.0; 

	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}  