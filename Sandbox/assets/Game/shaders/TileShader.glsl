#type vertex
#version 420 core
layout (location = 0) in vec3 a_Positions;
layout (location = 1) in vec3 a_Normals;
layout (location = 2) in vec2 a_TexCoords;

layout (std140, binding = 0) uniform u_Data
{
	mat4 u_ViewProjectionMatrix;
	vec3 u_CameraPosition;
};

out vec2 TexCoord;

uniform mat4 u_Transform;

void main()
{
	TexCoord = a_TexCoords;
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Positions, 1.0);
}



#type fragment
#version 420 core
out vec4 color;


uniform sampler2D texture_diffuse1;

in vec2 TexCoord;


void main()
{	
	color = texture(texture_diffuse1, TexCoord);	
}