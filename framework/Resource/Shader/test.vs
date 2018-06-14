#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (std140) uniform shareBlock
{               // 基准对齐量   // 对齐偏移量
    mat4 P;     // 16           0
    mat4 V;     // 16           16*4
};

out vs{
    vec3 normal;
    vec2 texCoord;
}_out;

uniform mat4 M;

void main()
{
    gl_Position = P * V * M * vec4(aPos, 1.0);
    _out.normal = aNormal;
    _out.texCoord = aTexCoord;
}