#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
    TexCoords = aPos;
    vec4 pos = u_ProjectionMatrix * u_ViewMatrix * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  

#type fragment
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube u_SkyboxTexture;

void main()
{    
    FragColor = texture(u_SkyboxTexture, TexCoords);
}