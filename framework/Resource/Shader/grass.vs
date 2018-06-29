#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 posAndSize;

out vec2 TexCoord;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 eyeFront;
uniform vec3 eyeUp;
uniform vec3 eyeRight;

out vec2 TexCoords;

void main()
{
    mat4 translaM = mat4(vec4(1.0,0.0,0.0,0.0),vec4(0.0,1.0,0.0,0.0),vec4(0.0,0.0,1.0,0.0),vec4(posAndSize.x,posAndSize.y,posAndSize.z,1.0));
    mat4 rotaM = mat4(vec4(eyeRight,0.0),vec4(eyeUp,0.0),vec4(-1.0 * eyeFront,0.0),vec4(0.0,0.0,0.0,1.0));
    float size = posAndSize.w;
    vec3 scaledP = aPos * size;
    vec4 worldPos = translaM * rotaM * vec4(scaledP,1.0);

    TexCoords = vec2(aPos.x + 0.5,aPos.y + 0.5);
    gl_Position = projection * view * worldPos;
}
