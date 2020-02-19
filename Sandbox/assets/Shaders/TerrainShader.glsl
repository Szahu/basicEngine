#type vertex
#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_color;

flat out vec3 pass_colour;//The "flat" qualifier stops the colour from being interpolated over the triangles.

#define MAX_NR_POINT_LIGHTS 5
#define MAX_NR_SPOT_LIGHTS 5 

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

//simple diffuse lighting
vec3 calculateLighting(){
	
	vec3 lightDirection = vec3(u_SpotLights[0].Direction);
	vec3 lightColour = vec3(u_SpotLights[0].Diffuse);
	vec2 lightBias = vec2(0.3f, 0.8f);
	float brightness = max(dot(-lightDirection, in_normal), 0.0);
	return (lightColour * lightBias.x) + (brightness * lightColour * lightBias.y);
}

void main(void){

	gl_Position = u_ViewProjectionMatrix * vec4(in_position, 1.0);
	
	vec3 lighting = calculateLighting();
	pass_colour = in_color * lighting;

}

#type fragment
#version 330

flat in vec3 pass_colour;

out vec4 out_colour;


void main(void){

	out_colour = vec4(pass_colour, 1.0);

}