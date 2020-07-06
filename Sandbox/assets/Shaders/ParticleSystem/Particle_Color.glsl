#type vertex
#version 420 core
			
layout(location = 0) in vec2 a_Positions;
layout(location = 1) in vec2 a_TexCoords;
layout(location = 2) in vec4 a_col0;
layout(location = 3) in vec4 a_col1;
layout(location = 4) in vec4 a_col2;
layout(location = 5) in vec4 a_col3;
layout(location = 6) in vec4 a_Color;

layout (std140, binding = 0) uniform u_Data
{
	mat4 u_ViewProjectionMatrix;
	vec3 u_CameraPosition;
};			
			
uniform mat4 u_Transform;
uniform vec4 u_Color;

out vec4 color_pass;

void main() 
{
	mat4 transform = mat4(a_col0, a_col1, a_col2, a_col3);
	color_pass = a_Color;
	gl_Position = u_ViewProjectionMatrix * transform * vec4(vec3(a_Positions, 0.0), 1.0);
}


#type fragment
#version 420 core
	


layout(location = 0) out vec4 color;

in vec4 color_pass;	

void main() 
{
	color = color_pass;
}
						