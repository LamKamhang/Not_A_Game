#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vs{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
}_out;

void main()
{
    _out.normal = aNormal;
    _out.texCoord = aTexCoord;
    _out.position = aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}