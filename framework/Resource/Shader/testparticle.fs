#version 330 core
out vec4 FragColor;

in vec4 frag_color;
in vec2 TexCoords;

uniform sampler2D texture0;

void main()
{
    float intensity = 1.0 - pow(length(vec2(TexCoords.x-0.5,TexCoords.y-0.5)), 4.0);
    FragColor = frag_color * intensity;// * texture(texture0, TexCoords);
}
