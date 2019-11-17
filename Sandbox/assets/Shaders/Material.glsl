#type vertex
#version 330 core
layout (location = 0) in vec3 a_Positions;
layout (location = 1) in vec3 a_Normals;
layout (location = 2) in vec2 a_TexCoords;
//layout (location = 3) in vec3 a_Translations;

out VS_OUT
{
	vec2 TexCoords;
	vec3 Normal;
	vec3 FragPos;
	vec3 CameraPosition;
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
	vs_out.CameraPosition = u_CameraPosition;
	vs_out.FragPos = vec3(u_Transform * vec4(a_Positions, 1.0));
	vs_out.Normal = mat3(transpose(inverse(u_Transform))) * a_Normals;

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
} fs_in;


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

//uniform samplerCube u_SkyboxTexture;

void main()
{	
	if(u_FlatColor.x != 0 || u_FlatColor.y != 0 || u_FlatColor.z != 0) {color = vec4(u_FlatColor, 1.0);}

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
	
	vec4 diffTex = texture(texture_diffuse1, fs_in.TexCoords);
	if (diffTex.x != 0 || diffTex.y != 0 || diffTex.z != 0) {objectColor *= diffTex;}

	//float ratio = 1.00 / 2.42;
	//vec3 I = normalize(FragPos - u_CameraPosition);
    //vec3 R = refract(I, normalize(s_CommonData.c_Normal), ratio);
    //vec4 Reflection_Color = vec4(texture(u_SkyboxTexture, R).rgb, 1.0);


	color = objectColor * result;
	
	}
}

vec4 CalculatePointLight(PointLight light, CommonData data)
{
	// ambient
	vec3 ambient = u_Material.ambient * light.ambient;

	// diffuse 
	//vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - fs_in.FragPos);
	float diff = max(dot(data.c_Normal, lightDir), 0.0);
	vec3 diffuse = (diff * u_Material.diffuse) * light.diffuse;

	// specular
	vec3 reflectDir = reflect(-lightDir, data.c_Normal);
	float spec = pow(max(dot(data.c_ViewDirection, reflectDir), 0.0), 1 / u_Material.shininess * 1.28f);
	vec3 specular = light.specular * (spec * u_Material.specular);

	vec3 result = (ambient + diffuse + specular);
	return vec4(result, 1.0);
}