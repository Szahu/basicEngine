#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Positions;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;			
			
void main() 
{
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Positions, 1.0);
}


#type geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

out vec3 normal;

void main( void )
{
    vec3 a = ( gl_in[1].gl_Position - gl_in[0].gl_Position ).xyz;
    vec3 b = ( gl_in[2].gl_Position - gl_in[0].gl_Position ).xyz;
    vec3 N = normalize( cross( b, a ) );

    for( int i=0; i<gl_in.length( ); ++i )
    {
        gl_Position = gl_in[i].gl_Position;
        normal = N;
        EmitVertex( );
    }

    EndPrimitive( );
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

uniform vec4 u_Color;

in vec3 normal;

void main() 
{
	//color = u_Color + vec4(0.0f, 0.0f, 0.0, 1.0);
	color = vec4(normal, 1.0);
}
						