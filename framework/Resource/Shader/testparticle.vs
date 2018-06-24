#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 posAndSize;
layout (location = 2) in vec4 color;

uniform mat4 projection;
uniform mat4 view;

out vec4 frag_color;
out vec2 TexCoords;

void main()
{
    vec3 translation = vec3(posAndSize.x,posAndSize.y,posAndSize.z);
    float size = posAndSize.w;
    vec3 worldPos = (aPos * size + translation);
    
    frag_color = color;
    TexCoords = vec2(aPos.x + 0.5,aPos.y + 0.5);
    gl_Position = projection * view * vec4(worldPos,1.0);
}
