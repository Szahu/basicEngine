#type vertex
#version 330 core
layout (location = 0) in vec3 a_Positions;
layout (location = 1) in vec3 a_Normals;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in vec3 a_Tangents;
layout (location = 4) in vec3 a_Bitangents;  

out VS_OUT
{
	vec2 TexCoords;
	vec3 Normal;
	vec3 FragPos;
	vec3 CameraPosition;
	mat3 TBN;
	vec4 FragPosLightSpace;
} vs_out;

layout (std140, binding = 0) uniform u_Data
{
	mat4 u_ViewProjectionMatrix;
	vec3 u_CameraPosition;
};

uniform mat4 u_Transform;
uniform sampler2D texture_height1;
uniform bool u_HasHeightTexture;
uniform mat4 u_LightSpaceMatrix;

vec3 TEX_HEIGHT;

void main()
{ 
	vs_out.TexCoords = a_TexCoords;
	vs_out.FragPos = vec3(u_Transform * vec4(a_Positions, 1.0));
	vs_out.Normal = mat3(transpose(inverse(u_Transform))) * a_Normals;
	vs_out.CameraPosition = u_CameraPosition;

	vec3 T = normalize(vec3(u_Transform * vec4(a_Tangents,   0.0)));
    vec3 B = normalize(vec3(u_Transform * vec4(a_Bitangents, 0.0)));
    vec3 N = normalize(vec3(u_Transform * vec4(a_Normals,    0.0)));
    vs_out.TBN = mat3(T, B, N);
	
	if(u_HasHeightTexture) TEX_HEIGHT = texture(texture_height1, a_TexCoords).rgb; else TEX_HEIGHT = vec3(0.0);

	vec3 newPos = a_Normals * TEX_HEIGHT; 
	float range = 0.05f;
	vec4 new_Positions = vec4(a_Positions.x + newPos.x * range, a_Positions.y + newPos.y * range, a_Positions.z + newPos.z * range, 1.0f);
	
	vs_out.FragPosLightSpace = u_LightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

	//gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Positions, 1.0);
	gl_Position = u_ViewProjectionMatrix * u_Transform * new_Positions;
}


#type fragment
#version 330 core
out vec4 color;

in VS_OUT
{
	vec2 TexCoords;
	vec3 Normal;
	vec3 FragPos;
	vec3 CameraPosition;
	mat3 TBN;
	vec4 FragPosLightSpace;
} fs_in;

//HAS TO MATCH Max number of lights in Renderer.h!!
#define MAX_NR_POINT_LIGHTS 5
#define MAX_NR_SPOT_LIGHTS 5 

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material u_Material;

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

struct CommonData
{
	vec3 c_Normal;
	vec3 c_ViewDirection;
};

layout (std140, binding = 1) uniform u_Lights
{
	vec2 AmountOfLights; // .x are PointLights, .y are SpotLights
	PointLight[MAX_NR_POINT_LIGHTS] u_PointLights;
	SpotLight[MAX_NR_SPOT_LIGHTS] u_SpotLights;
};

uniform vec3 u_FlatColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_ambient1;

vec3 TEX_DIFFUSE;
vec3 TEX_SPECULAR;
vec3 TEX_NORMAL;
vec3 TEX_AMBIENT;

uniform bool u_HasDiffuseTexture;
uniform bool u_HasSpecularTexture;
uniform bool u_HasNormalTexture;
uniform bool u_HasAmbientTexture;

uniform samplerCube u_SkyboxTexture;

vec4 CalculatePointLight(PointLight light, CommonData data);
vec4 CalculateSpotLight(SpotLight light, CommonData data);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 Normal);

uniform sampler2D shadowMap;

void main()
{	
	if(u_FlatColor.x != 0 || u_FlatColor.y != 0 || u_FlatColor.z != 0) {color = vec4(u_FlatColor, 1.0);}
	else 
	{
	if(u_HasDiffuseTexture) TEX_DIFFUSE = texture(texture_diffuse1, fs_in.TexCoords).rgb;   else TEX_DIFFUSE = vec3(1.0);
	if(u_HasAmbientTexture) TEX_SPECULAR = texture(texture_specular1, fs_in.TexCoords).rgb; else TEX_AMBIENT = vec3(1.0);
	TEX_NORMAL = texture(texture_normal1, fs_in.TexCoords).rgb;
	if(u_HasAmbientTexture) TEX_AMBIENT = texture(texture_ambient1, fs_in.TexCoords).rgb;   else TEX_AMBIENT = vec3(1.0);


	// Setting up common Data
	CommonData s_CommonData;
	s_CommonData.c_Normal = normalize(fs_in.Normal);
	s_CommonData.c_ViewDirection = normalize(fs_in.CameraPosition - fs_in.FragPos);

	vec4 objectColor = vec4(1.0f);

	vec4 result = vec4(0.0);
	for(int i = 0;i < AmountOfLights.x;i ++)
	{
		result += CalculatePointLight(u_PointLights[i], s_CommonData);
	}
	
	for(int i = 0;i < AmountOfLights.y;i++)
	{
		result += CalculateSpotLight(u_SpotLights[i], s_CommonData);
	}

	//float ratio = 1.00 / 3.42;
	//vec3 I = normalize(FragPos - u_CameraPosition);
    //vec3 R = refract(I, normalize(s_CommonData.c_Normal), ratio);
    //vec4 Reflect_Color = vec4(texture(u_SkyboxTexture, R).rgb, 1.0);
	//
	//color = result + vec4(0.1) * Reflect_Color;
	
	float gamma = 1;

	color = vec4(result.rgb, 1.0f);
	}
}

vec4 CalculatePointLight(PointLight light, CommonData data)
{
	vec3 Norm;
	if(u_HasNormalTexture ) Norm = TEX_NORMAL; else Norm = data.c_Normal;

	// ambient
	vec3 ambient = u_Material.ambient * vec3(light.ambient) * TEX_AMBIENT;

	// diffuse 
	vec3 lightDir = normalize(vec3(light.position) - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + data.c_ViewDirection);
	float diff = max(dot(Norm, halfwayDir), 0.0);
	//float diff = max(dot(Norm, lightDir), 0.0);
	vec3 diffuse = (diff * u_Material.diffuse) * vec3(light.diffuse) * TEX_DIFFUSE;

	// specular
	vec3 reflectDir = reflect(-lightDir, Norm);
	float spec = pow(max(dot(data.c_ViewDirection, reflectDir), 0.0), (1 / u_Material.shininess * 1.28f));
	vec3 specular = vec3(light.specular) * (spec * u_Material.specular * TEX_SPECULAR) ;

	vec3 result = (ambient + diffuse + specular);
	return vec4(result, 1.0);
}

vec4 CalculateSpotLight(SpotLight light, CommonData data)
{
	vec3 Norm = TEX_NORMAL;

	vec3 ambient = u_Material.ambient * vec3(light.Ambient) * TEX_AMBIENT;
    
    // diffuse 
    vec3 lightDir = normalize(vec3(light.Position) - fs_in.FragPos);
    float diff = max(dot(data.c_Normal, lightDir), 0.0);
    vec3 diffuse = vec3(light.Diffuse) * u_Material.diffuse * TEX_DIFFUSE;  
    
    // specular
	vec3 halfwayDir = normalize(lightDir + data.c_ViewDirection);
    vec3 reflectDir = reflect(-lightDir, data.c_Normal);  
    float spec = pow(max(dot(data.c_ViewDirection, reflectDir), 0.0), (1 / u_Material.shininess * 1.28f));
    vec3 specular = vec3(light.Specular) * (spec * u_Material.specular * TEX_SPECULAR);  
    
    // spotlight (soft edges)
    float theta = dot(vec3(lightDir), normalize(-vec3(light.Direction))); 
    float epsilon = (light.InnerCutOff - light.OuterCutOff);
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(vec3(light.Position) - fs_in.FragPos);
    float attenuation = 5.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace, lightDir, Norm);  
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));   
    //vec3 result = ambient + diffuse + specular;
	//return vec4(result, 1.0f);

	return vec4(lighting, 1.0f);
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

