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
} vs_out;

layout (std140, binding = 0) uniform u_Data
{
	mat4 u_ViewProjectionMatrix;
	vec3 u_CameraPosition;
};

uniform mat4 u_Transform;

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

	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Positions, 1.0);
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
} fs_in;

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
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct CommonData
{
	vec3 c_Normal;
	vec3 c_ViewDirection;
};

layout (std140, binding = 1) uniform u_Lights
{
	PointLight[NR_POINT_LIGHTS] u_PointLights;
};

uniform vec3 u_FlatColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform samplerCube u_SkyboxTexture;

vec4 CalculatePointLight(PointLight light, CommonData data);

void main()
{	
	//if(u_FlatColor.x != 0 || u_FlatColor.y != 0 || u_FlatColor.z != 0) {color = vec4(u_FlatColor, 1.0);}
	if(false) {}
	else 
	{
	// Setting up common Data
	CommonData s_CommonData;
	s_CommonData.c_Normal = normalize(fs_in.Normal);
	s_CommonData.c_ViewDirection = normalize(fs_in.CameraPosition - fs_in.FragPos);

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
	//
	//color = result + vec4(0.1) * Reflect_Color;
	
	float gamma = 1;

	color = vec4(pow(result.rgb, vec3(1.0f / gamma)), 1.0f);
	}
}

vec4 CalculatePointLight(PointLight light, CommonData data)
{
	vec3 specTex = texture(texture_specular1, fs_in.TexCoords).rgb;

	vec3 diffTex1 = texture(texture_diffuse1, fs_in.TexCoords).rgb;

	vec3 normTex = texture(texture_normal1, fs_in.TexCoords).rgb;
	vec3 Norm = data.c_Normal;
	//if(normTex == vec3(1.0)) { Norm = data.c_Normal; }
	//else { vec3 Norm = normalize(normTex * 2.0 - 1.0); }

	// ambient
	vec3 ambient = u_Material.ambient * vec3(light.ambient) * diffTex1;

	// diffuse 
	vec3 lightDir = normalize(vec3(light.position) - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + data.c_ViewDirection);
	float diff = max(dot(Norm, halfwayDir), 0.0);
	//float diff = max(dot(Norm, lightDir), 0.0);
	vec3 diffuse = (diff * u_Material.diffuse) * vec3(light.diffuse) * diffTex1;

	// specular
	vec3 reflectDir = reflect(-lightDir, Norm);
	float spec = pow(max(dot(data.c_ViewDirection, reflectDir), 0.0), (1 / u_Material.shininess * 1.28f));
	vec3 specular = vec3(light.specular) * (spec * u_Material.specular * specTex) ;

	vec3 result = (ambient + diffuse + specular);
	return vec4(result, 1.0);
}