#version 330 core
out vec4 FragColor;

uniform sampler2D flameSpark;
uniform sampler2D flameStart;

in gs{
    int type;
    float age;
    float alpha;
    float size;
    float lifeTime;
    
    vec4 position;
    vec3 velocity;
}_out;

void main()
{
    vec4 texColor;
    if(_out.age > _out.lifeTime)discard;
    if(_out.age/_out.lifeTime<0.6)texColor=texture(flameSpark,gl_PointCoord);
    else texColor=texture(flameStart,gl_PointCoord);
    FragColor = texColor * vec4(0.5,0.3,0.1,_out.alpha);
}
