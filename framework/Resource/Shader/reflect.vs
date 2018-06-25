#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vs{
    vec3 Position;
    vec3 Normal;
    vec4 FragPosLightSpace;
}_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    _out.Normal = mat3(transpose(inverse(model))) * aNormal;
    _out.Position = vec3(model * vec4(aPos, 1.0));
    _out.FragPosLightSpace = lightSpaceMatrix * vec4(_out.Position, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
