#version 330 core
out vec4 FragColor;

in gs{
    vec3 direction;
    vec3 position;
    vec3 normal;
    vec2 texCoords;
}_in;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;

void main()
{   
    // float rate = 1.0;
    float dirt = dot(_in.direction, _in.direction);
    if (dirt > 10)
        discard;
    // else 
    //     rate = 1.5 / (dirt + 1) - 0.5;
    // Ambient Lighting
    vec3 ambient	=	0.2 * vec3(texture(texture_diffuse1, _in.texCoords));
    // Diffuse Lighting
    vec3 norm		=	normalize(_in.normal);
    vec3 lightDir	=	normalize(vec3(0, 50, 0) - _in.position);
    float diff		=	max(dot(norm,lightDir), 0.0);
    vec3 diffuse	=	diff * vec3(texture(texture_diffuse1, _in.texCoords));
    // Specular Lighting
    vec3 viewDir	=	normalize(viewPos - _in.position);
    vec3 reflectDir =	reflect(-lightDir, norm);
    float spec		=	pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular	=	0.1 * (spec * (vec3(1.0f, 1.0f, 1.0f)));
    vec3 result		=	ambient + diffuse + specular;
    FragColor		=	vec4(result, 1.0f);
}


