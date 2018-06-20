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

struct PointLight{
	vec3 color;
    float ambient;
	float diffuse;
	float specular;
};

#define NUM_POINT_LIGHTS 9
uniform PointLight point_light;
uniform vec3 point_light_pos[NUM_POINT_LIGHTS];

uniform Material floor_material;
uniform Material ceil_material;
uniform Material wall_material;

uniform vec3 viewPos;

// 0 --> floor, 1 --> wall
uniform bool floor_wall;

// function prototypes
vec3 CalcPointLight(vec3 pos, vec3 normal, vec3 fragPos, vec3 viewDir, Material material);

void main()
{   
    // some important variable
    vec3 norm = normalize(_in.normal);
    vec3 viewDir = normalize(viewPos - _in.position);
    vec3 result = vec3(0, 0, 0);
    // calculate the point light
    if (floor_wall)
    {
        for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
        {
            result += CalcPointLight(point_light_pos[i], norm, _in.position, viewDir, wall_material);
        }
    }
    else
    {
        if (gl_FrontFacing)
        {
            for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
            {
                result += CalcPointLight(point_light_pos[i], norm, _in.position, viewDir, floor_material);
            }
        }
        else
        {
            for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
            {
                result += CalcPointLight(point_light_pos[i], norm, _in.position, viewDir, ceil_material);
            }
        }
        
    }


    FragColor = vec4(result, 1.0);
}

/*
F_att = 1.0/(ax^2 + bx + c)
x = dist
dist    constant    linear      quardure
7       1.0         0.7         1.8
13      1.0         0.35        0.44
20      1.0         0.22        0.20
32      1.0         0.14        0.07
50      1.0         0.09        0.032
65      1.0         0.07        0.017
100     1.0         0.045       0.0075
160     1.0         0.027       0.0028
200     1.0         0.022       0.0019    
*/
vec3 CalcPointLight(vec3 pos, vec3 normal, vec3 fragPos, vec3 viewDir, Material material)
{
    vec3 lightDir = normalize(pos - fragPos);

    // phong algorithm to calculate this
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    //specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float dist = length(pos - fragPos);
    float attenuation = 1.0 / (1.0 + 0.07*dist + 0.017*dist*dist);

    // combine them
    vec3 ambient = point_light.ambient*point_light.color*material.ambient;
    vec3 diffuse = point_light.diffuse*point_light.color*material.diffuse*diff;
    vec3 specular = point_light.specular*point_light.color*material.specular*spec;
    
    return attenuation*(ambient + diffuse + specular);
}