#version 330 core
layout (location = 0) in vec3 aPos;

out vs{
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
}_out;

uniform int type;
uniform float age;
uniform float alpha;
uniform float size;
uniform float lifeTime;
uniform vec3 velocity;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    _out.type = type;
    _out.age=age;
    _out.alpha=alpha;
    _out.size=size;
    _out.lifeTime=lifeTime;
    _out.velocity = velocity;
    
    _out.model = model;
    _out.view = view;
    _out.projection = projection;

    gl_PointSize = size;
    _out.position = model * vec4(aPos, 1.0);;
}
