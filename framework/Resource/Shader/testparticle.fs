#version 330 core
out vec4 FragColor;

in vec4 frag_color;
in vec2 TexCoords;

uniform sampler2D texture0;

void main()
{
    FragColor = frag_color;// * texture(texture0, TexCoords);
}
