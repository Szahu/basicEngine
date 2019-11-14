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

#define NR_POINT_LIGHTS 1

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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform samplerCube u_SkyboxTexture;

vec4 CalculatePointLight(PointLight light, CommonData data);

void main()
{	
	if(u_FlatColor.x != 0 || u_FlatColor.y != 0 || u_FlatColor.z != 0) {color = vec4(u_FlatColor, 1.0);}

	else 
	{
	// Setting up common Data
	CommonData s_CommonData;
	s_CommonData.c_Normal = normalize(Normal);
	s_CommonData.c_ViewDirection = normalize(u_CameraPosition - FragPos);

	vec4 objectColor = vec4(1.0f);
		
	float ratio = 1.00 / 2.42;
	vec3 I = normalize(FragPos - u_CameraPosition);
    vec3 R = refract(I, normalize(s_CommonData.c_Normal), ratio);
    //vec4 Reflected_Color = vec4(texture(u_SkyboxTexture, R).rgb, 1.0);

	vec4 result = vec4(0.0);
	for(int i = 0;i < u_PointLights.length();i ++)
	{
		result += CalculatePointLight(u_PointLights[i], s_CommonData);
	}
	
	color = objectColor * result;
	
	}
}

vec4 CalculatePointLight(PointLight light, CommonData data)
{
	vec3 specTex = texture(texture_specular1, TexCoords).rgb;
	if (specTex.x == 0 && specTex.y == 0 && specTex.z == 0) { specTex = vec3(1.0); }

	vec3 diffTex = texture(texture_diffuse1, TexCoords).rgb;
	if (diffTex.x == 0 && diffTex.y == 0 && diffTex.z == 0) { diffTex = vec3(1.0); }

	// ambient
	vec3 ambient = u_Material.ambient * light.ambient * diffTex;

	// diffuse 
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(data.c_Normal, lightDir), 0.0);
	vec3 diffuse = (diff * u_Material.diffuse) * light.diffuse * diffTex;

	// specular
	vec3 reflectDir = reflect(-lightDir, data.c_Normal);
	float spec = pow(max(dot(data.c_ViewDirection, reflectDir), 0.0), (1 / u_Material.shininess * 1.28f));
	vec3 specular = light.specular * (spec * u_Material.specular * specTex.x);

	vec3 result = (ambient + diffuse + specular);
	return vec4(result, 1.0);
}