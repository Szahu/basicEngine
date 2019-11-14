#type vertex
#version 330 core
layout (location = 0) in vec3 a_Positions;
layout (location = 1) in vec3 a_Normals;
layout (location = 2) in vec2 a_TexCoords;
//layout (location = 3) in vec3 a_Translations;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

void main()
{
	TexCoords = a_TexCoords;

	FragPos = vec3(u_Transform * vec4(a_Positions, 1.0));

	Normal = mat3(transpose(inverse(u_Transform))) * a_Normals;

	//gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Positions.x + a_Translations.x, a_Positions.y + a_Translations.y, a_Positions.z + a_Translations.z, 1.0);
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Positions, 1.0);
}



#type fragment
#version 330 core
out vec4 color;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

#define NR_POINT_LIGHTS 2

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
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct CommonData
{
	vec3 c_Normal;
	vec3 c_ViewDirection;
};

uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform PointLight u_PointLight;
uniform vec3 u_FlatColor;

vec4 CalculatePointLight(PointLight light, CommonData data);

uniform samplerCube u_SkyboxTexture;

void main()
{	
	// Setting up common Data
	CommonData s_CommonData;
	s_CommonData.c_Normal = normalize(Normal);
	s_CommonData.c_ViewDirection = normalize(u_CameraPosition - FragPos);

	float ratio = 1.00 / 2.42;
	vec3 I = normalize(FragPos - u_CameraPosition);
    vec3 R = refract(I, normalize(s_CommonData.c_Normal), ratio);
    color = vec4(texture(u_SkyboxTexture, R).rgb, 1.0);
}

vec4 CalculatePointLight(PointLight light, CommonData data)
{
	// ambient
	vec3 ambient = u_Material.ambient * light.ambient;

	// diffuse 
	//vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(data.c_Normal, lightDir), 0.0);
	vec3 diffuse = (diff * u_Material.diffuse) * light.diffuse;

	// specular
	vec3 reflectDir = reflect(-lightDir, data.c_Normal);
	float spec = pow(max(dot(data.c_ViewDirection, reflectDir), 0.0), 1 / u_Material.shininess * 1.28f);
	vec3 specular = light.specular * (spec * u_Material.specular);

	vec3 result = (ambient + diffuse + specular);
	return vec4(result, 1.0);
}