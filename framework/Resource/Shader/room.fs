#version 330 core
out vec4 FragColor;

in vs{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
}_in;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D floor_tex;
uniform sampler2D ceil_tex;
uniform sampler2D wall_tex;
uniform bool floor_wall;

void main()
{   
    vec3 color;
    if (floor_wall)
    {
        color = vec3(texture(wall_tex, _in.texCoord));
    }
    else
    {
        if(gl_FrontFacing)
            color = vec3(texture(ceil_tex, _in.texCoord));
        else
            color = vec3(texture(floor_tex, _in.texCoord));
    }

    // Ambient Lighting
    vec3 ambient	=	0.1 * color;
    // Diffuse Lighting
    vec3 norm		=	normalize(_in.normal);
    vec3 lightDir	=	normalize(lightPos - _in.position);
    float diff		=	max(dot(norm, lightDir), 0.0);
    vec3 diffuse	=	0.5 * diff * color;
    // Specular Lighting
    vec3 viewDir	=	normalize(viewPos - _in.position);
    vec3 reflectDir =	reflect(-lightDir, norm);
    float spec		=	pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular	=	0.4 * (spec * color);
    vec3 result		=	ambient + diffuse + specular;
    FragColor		=	vec4(result, 1.0f);
}
