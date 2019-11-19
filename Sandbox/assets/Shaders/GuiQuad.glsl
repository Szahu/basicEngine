#type vertex
#version 330 core
			
layout(location = 0) in vec2 a_Positions;
layout(location = 1) in vec2 a_TexCoords;

layout (std140, binding = 0) uniform u_Data
{
	mat4 u_ViewProjectionMatrix;
	vec3 u_CameraPosition;
};			
			
uniform mat4 u_Transform;
uniform vec3 u_CameraRight;
uniform vec3 u_CameraUp;

out vec2 TexCoord;

void main() 
{
	TexCoord = a_TexCoords;
	vec3 Positions = vec3(a_Positions, 0.0f);
	vec3 pos_worldspace = u_CameraRight * Positions.x * 1.0f + u_CameraUp * Positions.y * 1.0f;

	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(pos_worldspace, 1.0);
}


#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

uniform sampler2D texture_sample;
	
in vec2 TexCoord;

void main() 
{
	color = texture(texture_sample, TexCoord);
}
						