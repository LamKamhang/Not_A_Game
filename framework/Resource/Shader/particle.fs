#version 330 core
out vec4 FragColor;

uniform sampler2D flameSpark;
// uniform sampler2D flameStart;

in gs {
    float age;
    float alpha;
    float size;
    float lifeTime;
    // vec4 position;
    // vec3 velocity;
}_in;

void main()
{
    // vec4 texColor;
    if(_in.age > _in.lifeTime)discard;
    // if(_in.age/_in.lifeTime<0.6)texColor=texture(flameSpark,gl_PointCoord);
    // else texColor=texture(flameStart,gl_PointCoord);
    FragColor = texture(flameSpark,gl_PointCoord) * vec4(0.6,0.3,0.1,1.0);
}
