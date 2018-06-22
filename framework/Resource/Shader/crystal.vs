#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

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
    _out.texCoords = aTexCoord;
    _out.position = projection * view * model * vec4(aPos, 1.0);
}
