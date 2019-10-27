#type vertex
#version 330 core
	
	
layout(location = 0) in vec3 a_Positions;
layout(location = 1) in vec2 a_TexCoords;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;
						
out vec2 v_TexCoord;
			
void main() 
{
	v_TexCoord = a_TexCoords;
	gl_Position = u_ViewProjectionMatrix * u_Transform *  vec4(a_Positions, 1.0);
}
							

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;

in vec2 v_TexCoord;

void main() 
{
	color = texture(u_Texture, v_TexCoord);
}
		
