#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vs{
    vec3 position;
    vec3 normal;
    vec2 texCoords;
}_out;




void main()
{
    // _out.normal =  mat3(transpose(inverse(model))) * aNormal;
    _out.normal = aNormal;
    _out.texCoords = aTexCoord;
    _out.position =aPos;
}
