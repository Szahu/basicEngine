#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Positions;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;
						

out vec4 v_Color;
			
void main() 
{
	v_Color = a_Color;
	gl_Position = u_ViewProjectionMatrix * u_Transform *  vec4(a_Positions, 1.0);
}


#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec4 v_Color;

uniform vec4 u_Color;

void main() 
{
	color = v_Color * (u_Color + vec4(0.0, 0.0, 0.0, 1.0));
}
						