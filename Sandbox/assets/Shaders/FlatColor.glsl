#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Positions;
layout(location = 1) in vec3 a_Translations;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;			
			
void main() 
{
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Positions.x + a_Translations.x, a_Positions.y, a_Positions.z + a_Translations.z, 1.0);
}


#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main() 
{
	color = u_Color + vec4(0.0, 0.0, 0.0, 1.0);
}
						