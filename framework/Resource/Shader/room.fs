#version 330 core
out vec4 FragColor;

#define NUM_POINT_LIGHTS 9

in vs{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    vec3 point_light_pos[NUM_POINT_LIGHTS];
    //vec4 FragPosLightSpace;
}_in;

struct Material{
    bool diffuse_tex_color;
    bool specular_tex_color;
    sampler2D diffuse_tex;
    sampler2D specular_tex;
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

struct DirLight{
    vec3 dir;
    vec3 color;
    float ambient;
    float diffuse;
    float specular;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 color;
    float ambient;
    float diffuse;
    float specular;
};

//uniform sampler2D shadowMap;
uniform SpotLight spot_light;
uniform DirLight dir_light;
uniform PointLight point_light;
uniform Material floor_material;
uniform Material ceil_material;
uniform Material wall_material;

uniform bool flashlight_on;

uniform vec3 viewPos;

// 0 --> floor, 1 --> wall
uniform bool floor_wall;

// 0 --> bilinn, 1 --> phong
uniform bool phong;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, Material material);
vec3 CalcPointLight(vec3 pos, vec3 normal, vec3 fragPos, vec3 viewDir, Material material);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material material);
//float ShadowCalculation();

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
            result += CalcPointLight(_in.point_light_pos[i], norm, _in.position, viewDir, wall_material);
        }
        result += CalcDirLight(dir_light, norm, viewDir, wall_material);
        result += CalcSpotLight(spot_light, norm, _in.position, viewDir, wall_material);
    }
    else
    {
        if (gl_FrontFacing)
        {
            for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
            {
                result += CalcPointLight(_in.point_light_pos[i], norm, _in.position, viewDir, floor_material);
            }
            result += CalcDirLight(dir_light, norm, viewDir, floor_material);
            result += CalcSpotLight(spot_light, norm, _in.position, viewDir, floor_material);
        }
        else
        {
            for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
            {
                result += CalcPointLight(_in.point_light_pos[i], norm, _in.position, viewDir, ceil_material);
            }
            result += CalcDirLight(dir_light, norm, viewDir, ceil_material);
            result += CalcSpotLight(spot_light, norm, _in.position, viewDir, ceil_material);
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
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    float spec;

    if (phong)
    {        
        //specular
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);        
    }else{
        // Blinn-phong algorithm to calculate this
        //specular
        vec3 halfDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfDir), 0.0), material.shininess);
    }
    // attenuation
    float dist = length(pos - fragPos);
    float attenuation = 1.0 / (1.0 + 0.07*dist + 0.017*dist*dist);

    // combine them
    vec3 ambient, diffuse, specular;
    if (material.diffuse_tex_color)
    {
        ambient = vec3(texture(material.diffuse_tex, _in.texCoord))*point_light.ambient*point_light.color;
        diffuse = vec3(texture(material.diffuse_tex, _in.texCoord))*point_light.diffuse*point_light.color*diff;
    }
    else
    {
        ambient = point_light.ambient*point_light.color*material.ambient;
        diffuse = point_light.diffuse*point_light.color*material.diffuse*diff;
    }

    if (material.specular_tex_color)
    {
        specular = vec3(texture(material.specular_tex, _in.texCoord))*point_light.specular*point_light.color*spec;
    }
    else
    {
        vec3 specular = point_light.specular*point_light.color*material.specular*spec;
    }
    
    return attenuation*(ambient + diffuse + specular);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, Material material)
{
    vec3 lightDir = normalize(-light.dir);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    float spec;
    if (phong)
    {
        // phong algorithm to calculate this
        //specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }else{
        vec3 halfDir = normalize(lightDir + viewDir);
        // Blinn-phong algorithm to calculate this
        //specular
        float spec = pow(max(dot(normal, halfDir), 0.0), material.shininess);
    }
    // combine them
    vec3 ambient, diffuse, specular;
    if (material.diffuse_tex_color)
    {
        ambient = vec3(texture(material.diffuse_tex, _in.texCoord))*light.ambient*light.color;
        diffuse = vec3(texture(material.diffuse_tex, _in.texCoord))*light.diffuse*light.color*diff;
    }
    else
    {
        ambient = light.ambient*light.color*material.ambient;
        diffuse = light.diffuse*light.color*material.diffuse*diff;
    }

    if (material.specular_tex_color)
    {
        specular = vec3(texture(material.specular_tex, _in.texCoord))*light.specular*light.color*spec;
    }
    else
    {
        vec3 specular = light.specular*light.color*material.specular*spec;
    }

    // 计算阴影
    //float shadow = ShadowCalculation();
    //return ambient + (1.0 - shadow)*(diffuse + specular);
    return ambient + diffuse + specular;
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
// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material material)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    float spec;
    if (phong)
    {
        // phong algorithm to calculate this
        //specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }else{
        vec3 halfDir = normalize(lightDir + viewDir);
        // Blinn-phong algorithm to calculate this
        //specular
        float spec = pow(max(dot(normal, halfDir), 0.0), material.shininess);
    }
    // attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + 0.007 * dist +0.0018 * (dist * dist)); 
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = cos(0.1744) - cos(0.20);
    float intensity = clamp((theta - cos(0.20)) / epsilon, 0.0, 1.0);
    // float epsilon = light.cutOff - light.outerCutOff;
    // float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine them
    vec3 ambient, diffuse, specular;
    if (material.diffuse_tex_color)
    {
        ambient = vec3(texture(material.diffuse_tex, _in.texCoord))*light.ambient*light.color;
        diffuse = vec3(texture(material.diffuse_tex, _in.texCoord))*light.diffuse*light.color*diff;
    }
    else
    {
        ambient = light.ambient*light.color*material.ambient;
        diffuse = light.diffuse*light.color*material.diffuse*diff;
    }

    if (material.specular_tex_color)
    {
        specular = vec3(texture(material.specular_tex, _in.texCoord))*light.specular*light.color*spec;
    }
    else
    {
        vec3 specular = light.specular*light.color*material.specular*spec;
    }
    
    return attenuation * intensity * (ambient + diffuse + specular);
}

// float ShadowCalculation()
// {
//     // 执行透视除法
//     vec3 projCoords = (_in.FragPosLightSpace.xyz / _in.FragPosLightSpace.w)*0.5+0.5;
//     // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
//     float closestDepth = texture(shadowMap, projCoords.xy).r; 
//     // 取得当前片元在光源视角下的深度
//     float currentDepth = projCoords.z;
//     // 检查当前片元是否在阴影中
//     float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

//     return shadow;
// }