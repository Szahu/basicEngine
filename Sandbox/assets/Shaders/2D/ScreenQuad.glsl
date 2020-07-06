#type vertex
#version 420 core
			
layout(location = 0) in vec3 a_Positions;
layout(location = 1) in vec2 a_TexCoords;

out vec2 TexCoords;

void main() 
{
	TexCoords = a_TexCoords;
	gl_Position = vec4(a_Positions, 1.0);
}


#type fragment
#version 420 core
			
layout(location = 0) out vec4 color;

in vec2 TexCoords;
uniform sampler2D tex;

void main() 
{
	color = texture(tex, TexCoords);
}
						