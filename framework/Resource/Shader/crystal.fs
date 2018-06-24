#version 400 core
out vec4 FragColor;

in gs{
    vec3 direction;
    vec3 position;
    vec3 normal;
    vec2 texCoords;
}_in;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform vec3 color;

void main()
{   
    float dirt = dot(_in.direction, _in.direction);
    if (dirt > 10)discard;
    vec3 I = normalize(_in.position - cameraPos);
    // vec3 R = normalize( reflect(I, normalize(Normal)) );
    float ratio = 1.00 / 1.33;
    vec3 R = refract(I, normalize(_in.normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb * color, 1.0);
}
