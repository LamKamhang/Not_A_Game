#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vs{
    vec4 position;
    vec3 normal;
    vec2 texCoords;
}_out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    _out.normal =  mat3(transpose(inverse(model))) * aNormal;
    _out.texCoords = aTexCoords;
    _out.position = projection * view * model * vec4(aPos, 1.0);
}


//|||||||||||||||||||||||||||||||||||||||||||||||
// old version
// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoord;

// out vs{
//     vec3 position;
//     vec3 normal;
//     vec2 texCoord;
// }_out;


// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

// void main()
// {
//     _out.position = aPos;
//     _out.normal = aNormal;
//     _out.texCoord = aTexCoord;
//     gl_Position = projection * view * model * vec4(aPos, 1.0);
// }