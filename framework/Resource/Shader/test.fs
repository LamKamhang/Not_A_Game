#version 330 core
out vec4 FragColor;

in vs{
    vec3 normal;
    vec2 texCoord;
}_in;

uniform sampler2D color;

void main()
{
    if (gl_FrontFacing)
        FragColor = vec4(1, 0, 1, 1);
    else
        FragColor = texture(color, _in.texCoord);
    // if (color.a < 0.1)
    //     discard;
    // else
    //     FragColor = color;
}