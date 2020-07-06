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
			
layout(location = 0) out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D tex;

uniform vec3 u_Color;

const float offset = 1.0 / 300.0;  

void main() 
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(tex, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    FragColor = vec4(col, 1.0);
}
						