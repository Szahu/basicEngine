#type vertex
#version 330 core
layout (location = 0) in vec3 a_Positions;
layout (location = 1) in vec3 a_Normals;
layout (location = 2) in vec2 a_TexCoords;

out VS_OUT
{
	vec2 TexCoords;
	vec3 Normal;
	vec3 FragPos;
} vs_out;

layout (std140, binding = 0) uniform u_Data
{
	mat4 ViewProjectionMatrix;
};

uniform mat4 u_Transform;

void main()
{
	vs_out.TexCoords = a_TexCoords;
	vs_out.FragPos = vec3(u_Transform * vec4(a_Positions, 1.0));
	vs_out.Normal = mat3(transpose(inverse(u_Transform))) * a_Normals;

	gl_Position = ViewProjectionMatrix * u_Transform * vec4(a_Positions, 1.0);
}



#type fragment
#version 330 core
out vec4 color;

in VS_OUT
{
	vec2 TexCoords;
	vec3 Normal;
	vec3 FragPos;
} fs_in;


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
uniform vec3 u_FlatColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
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
	s_CommonData.c_Normal = normalize(fs_in.Normal);
	s_CommonData.c_ViewDirection = normalize(u_CameraPosition - fs_in.FragPos);

	vec4 objectColor = vec4(1.0f);

	vec4 result = vec4(0.0);
	for(int i = 0;i < u_PointLights.length();i ++)
	{
		result += CalculatePointLight(u_PointLights[i], s_CommonData);
	}
	
	//float ratio = 1.00 / 3.42;
	//vec3 I = normalize(FragPos - u_CameraPosition);
    //vec3 R = refract(I, normalize(s_CommonData.c_Normal), ratio);
    //vec4 Reflect_Color = vec4(texture(u_SkyboxTexture, R).rgb, 1.0);

	//color = result + vec4(0.1) * Reflect_Color;
	color = objectColor * result;
	}
}

vec4 CalculatePointLight(PointLight light, CommonData data)
{
	vec3 specTex = texture(texture_specular1, fs_in.TexCoords).rgb;
	if (specTex.x == 0 && specTex.y == 0 && specTex.z == 0) { specTex = vec3(1.0); }

	vec3 diffTex1 = texture(texture_diffuse1, fs_in.TexCoords).rgb;
	if (diffTex1.x == 0 && diffTex1.y == 0 && diffTex1.z == 0) { diffTex1 = vec3(1.0); }

	vec3 diffTex2 = texture(texture_diffuse2, fs_in.TexCoords).rgb;
	if (diffTex2.x == 0 && diffTex2.y == 0 && diffTex2.z == 0) { diffTex2 = vec3(1.0); }

	// ambient
	vec3 ambient = u_Material.ambient * light.ambient * diffTex1 * diffTex2;

	// diffuse 
	vec3 lightDir = normalize(light.position - fs_in.FragPos);
	float diff = max(dot(data.c_Normal, lightDir), 0.0);
	vec3 diffuse = (diff * u_Material.diffuse) * light.diffuse * diffTex1 * diffTex2;

	// specular
	vec3 reflectDir = reflect(-lightDir, data.c_Normal);
	float spec = pow(max(dot(data.c_ViewDirection, reflectDir), 0.0), (1 / u_Material.shininess * 1.28f));
	vec3 specular = light.specular * (spec * u_Material.specular) * specTex;

	vec3 result = (ambient + diffuse + specular);
	return vec4(result, 1.0);
}