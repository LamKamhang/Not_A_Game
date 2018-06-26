#version 330 core
out vec4 FragColor;

#define NUM_POINT_LIGHTS 9

in vs{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    vec4 FragPosLightSpace;
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


struct DirLight{
    vec3 dir;
    vec3 color;
    float ambient;
    float diffuse;
    float specular;
};


uniform sampler2D shadowMap;
uniform DirLight dir_light;
uniform Material floor_material;

uniform vec3 viewPos;

// 0 --> bilinn, 1 --> phong
uniform bool phong;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, Material material);
float ShadowCalculation(vec3 normal);

void main()
{   
    // some important variable
    vec3 norm = normalize(_in.normal);
    vec3 viewDir = normalize(viewPos - _in.position);
    vec3 result = vec3(0, 0, 0);

    result += CalcDirLight(dir_light, norm, viewDir, floor_material);            

    FragColor = vec4(result, 1.0);
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
    float shadow = ShadowCalculation(normal);
    return ambient + (1-shadow)*(diffuse + specular);
}


float ShadowCalculation(vec3 normal)
{
    float shadow;
    float bias = 0.002;
    // 执行透视除法
    vec3 projCoords = (_in.FragPosLightSpace.xyz / _in.FragPosLightSpace.w)*0.5+0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)

    if (projCoords.z > 1.0)
        shadow = 0.0;
    else
    {
        float closestDepth = texture(shadowMap, projCoords.xy).r; 
        // 取得当前片元在光源视角下的深度
        float currentDepth = projCoords.z;
        // // 检查当前片元是否在阴影中
        // shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
        vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
            }    
        }
        shadow /= 9.0;
    }
    

    return shadow;
}