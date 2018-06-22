#version 330 core
layout (points) in;
layout (points,max_vertices = 10) out;

in fs{
    int type;
    float age;
    float alpha;
    float size;
    float lifeTime;
    
    vec4 position;
    vec3 velocity;
}_in[];

out gs{
    int type;
    float age;
    float alpha;
    float size;
    float lifeTime;
    
    vec4 position;
    vec3 velocity;
}_out;

#define FLAME_LANUCHER 1
#define FLAME_SHELL 2

#define PARTICLE_MAX_LIFE 5.0f
#define PARTICLE_MIN_LIFE 0.2f

uniform float time;

float Rand2(float x,float y)
{
    return fract(cos(x * (12.9898) + y * (4.1414)) * 43758.5453);
}

void main()
{
    if(_in[0].type == FLAME_LANUCHER){
        // lanucher
        _out.type = FLAME_LANUCHER;
        _out.age = _in[0].age;
        _out.alpha = _in[0].alpha;
        
        gl_PointSize = _out.size;
        _out.size = _in[0].size;
        _out.lifeTime= _in[0].lifeTime;
        
        _out.position = _in[0].position;
        gl_Position = vec4(_out.position,1.0);
        _out.velocity = _in[0].velocity;
        EmitVertex();

        _out.type = FLAME_LANUCHER;
        _out.age = time;
        _out.alpha = _in[0].alpha;
        gl_PointSize = _out.size;
        _out.size = _in[0].size;
        _out.lifeTime = (PARTICLE_MAX_LIFE-PARTICLE_MIN_LIFE)*Rand2(_in[0].velocity,_in[0].velocity)+PARTICLE_MIN_LIFE;
        
        _out.position = _in[0].position + _in[0].velocity * time;
        gl_Position = vec4(_out.position,1.0);
        _out.velocity = _in[0].velocity;
        EmitVertex();
        EndPrimitive();
    }
}