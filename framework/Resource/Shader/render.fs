#version 330 core
out vec4 FragColor;

in vs{
    vec2 TexCoords;
}_in;

uniform sampler2D screenTexture;

uniform bool Inversion;
void main()
{
    if (Inversion)
        FragColor = vec4(vec3(1.0 - texture(screenTexture, _in.TexCoords)), 1.0);
    else
        FragColor = texture(screenTexture, _in.TexCoords);
}