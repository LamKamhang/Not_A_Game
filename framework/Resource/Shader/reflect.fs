#version 330 core
out vec4 FragColor;

in vs{
    vec3 Position;
    vec3 Normal;
    vec4 FragPosLightSpace;
}_in;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D shadowMap;

float ShadowCalculation();

void main()
{             
    vec3 I = normalize(_in.Position - cameraPos);
    // vec3 R = normalize( reflect(I, normalize(Normal)) );
    float ratio = 1.00 / 1.33;
    vec3 R = refract(I, normalize(_in.Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb * vec3(1.0,0.3,0.3), 1.0);
}

float ShadowCalculation()
{
    // 执行透视除法
    vec3 projCoords = (_in.FragPosLightSpace.xyz / _in.FragPosLightSpace.w)*0.5+0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // 取得当前片元在光源视角下的深度
    float currentDepth = projCoords.z;
    // 检查当前片元是否在阴影中
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}