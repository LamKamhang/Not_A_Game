// #version 330 core
// out vec4 FragColor;

// in vs{
//     vec3 normal;
//     vec2 texCoord;
// }_in;


// void main()
// {
//     if (gl_FrontFacing)
//         FragColor = vec4(1, 0, 0, 1);
//     else
//         FragColor = vec4(0, 1, 0, 1);
//     // if (color.a < 0.1)
//     //     discard;
//     // else
//     //     FragColor = color;
// }

#version 330 core
out vec4 FragColor;

void main()
{
    FragColor=vec4(1.0,0.0,0.0,1.0);
}