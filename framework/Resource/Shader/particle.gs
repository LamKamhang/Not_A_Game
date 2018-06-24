#version 330 core
layout (points) in;
layout (points, max_vertices = 10) out;

in vs{
    int type;
    float age;
    float alpha;
    float size;
    float lifeTime;
    
    vec4 position;
    vec3 velocity;

    mat4 model;
    mat4 view;
    mat4 projection;
}_in[];

out gs{
    float age;
    float alpha;
    float size;
    float lifeTime;
    
    // vec4 position;
    // vec3 velocity;
}_out;

#define FLAME_LANUCHER 0
#define FLAME_SHELL 1

#define PARTICLE_MAX_LIFE 5.0f
#define PARTICLE_MIN_LIFE 0.2f

uniform float initTime;
uniform float time;

float Rand2(float x,float y)
{
    return fract(cos(x * (12.9898) + y * (4.1414)) * 43758.5453);
}

void main()
{
    if(_in[0].type == FLAME_LANUCHER){
        gl_PointSize = gl_in[0].gl_PointSize;

        // lanucher
        _out.age = _in[0].age;
        _out.alpha = _in[0].alpha;
        _out.size = _in[0].size;
        _out.lifeTime= _in[0].lifeTime;
        gl_Position = _in[0].projection * _in[0].view * _in[0].position;
        EmitVertex();

        // _out.age = (time - initTime) * 0.1;
        // _out.alpha = _in[0].alpha;
        // _out.size = _in[0].size * 10.0/(time - initTime);
        // gl_PointSize = _out.size;
        // _out.lifeTime = (PARTICLE_MAX_LIFE - PARTICLE_MIN_LIFE)*Rand2(time,time)+PARTICLE_MIN_LIFE;
        gl_Position = _in[0].projection * _in[0].view * vec4(vec3(_in[0].position) + _in[0].velocity * (time-initTime) * 0.05, 1.0);
        EmitVertex();
        EndPrimitive();
    }
}