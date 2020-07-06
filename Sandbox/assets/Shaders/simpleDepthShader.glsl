#type vertex
#version 420 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 u_Transform;

layout (std140, binding = 0) uniform u_Data
{
	mat4 u_ViewProjectionMatrix;
	vec3 u_CameraPosition;
};


void main()
{
    gl_Position = lightSpaceMatrix * u_Transform * vec4(aPos, 1.0);
} 

#type fragment
#version 420 core
out vec4 color;

void main()
{
	color = vec4(1.0f);
}  