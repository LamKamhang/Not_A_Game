#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(Position - cameraPos);
    // vec3 R = normalize( reflect(I, normalize(Normal)) );
    float ratio = 1.00 / 1.33;
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb * vec3(1.0,0.3,0.3), 1.0);
}
