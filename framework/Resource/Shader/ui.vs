#version 330 core
layout (location = 0) in vec3 aPos;
out vec2 TexCoords;
void main()
{
    TexCoords = vec2(aPos.x,aPos.y);
    vec4 pos = vec4(aPos, 1.0);
    gl_Position = pos;
}
