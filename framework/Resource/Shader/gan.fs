#version 430 core
out vec4 FragColor;

in vs{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
}_in;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;
uniform sampler2D texture_diffuse5;
uniform sampler2D texture_diffuse6;
uniform sampler2D texture_diffuse7;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;
uniform sampler2D texture_specular4;
uniform sampler2D texture_specular5;

void main()
{    
    // Ambient Lighting
    vec3 ambient	=	0.2 * vec3(texture(texture_diffuse1, _in.texCoord));
    // Diffuse Lighting
    vec3 norm		=	normalize(_in.normal);
    vec3 lightDir	=	normalize(vec3(0, 50, 0) - _in.position);
    float diff		=	max(dot(norm,lightDir), 0.0);
    vec3 diffuse	=	diff * vec3(texture(texture_diffuse1, _in.texCoord));
    // Specular Lighting
    vec3 viewDir	=	normalize(viewPos - _in.position);
    vec3 reflectDir =	reflect(-lightDir, norm);
    float spec		=	pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular	=	0.1 * (spec * (vec3(1.0f, 1.0f, 1.0f)));
    vec3 result		=	ambient + diffuse + specular;
    FragColor		=	vec4(result, 1.0f);
}
