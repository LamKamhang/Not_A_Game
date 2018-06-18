#version 330 core
out vec4 FragColor;
in vec3 TexCoords;
// uniform samplerCube skybox;
uniform float alpha;
void main()
{   //texture(skybox, TexCoords);
    FragColor = vec4(0.8f,0.0f,0.0f,alpha*length(TexCoords));
}
