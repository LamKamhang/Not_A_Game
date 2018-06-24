#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = vec4(vec3(1.0 - texture(skybox, TexCoords)), 1.0);
}
