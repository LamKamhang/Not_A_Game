#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vs{
    vec4 position;
    vec3 normal;
    vec2 texCoords;
}_in[];

out gs{
    vec3 direction;
    vec3 position;
    vec3 normal;
    vec2 texCoords;
}_out;

uniform float initTime;
uniform float time;
uniform bool explode_now;

vec4 explode(vec4 position, vec3 normal)
{
    // * 0.001
    vec3 direction = normal * 3* sqrt((time - initTime));
    _out.direction = direction;
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
   vec3 a = vec3(_in[0].position) - vec3(_in[1].position);
   vec3 b = vec3(_in[2].position) - vec3(_in[1].position);
   return normalize(cross(a, b));
}

void main() {    
    if (explode_now)
    {
        vec3 normal = GetNormal();
        gl_Position = explode(_in[0].position, normal);
        _out.position = vec3(gl_Position);
        _out.texCoords = _in[0].texCoords;
        _out.normal = _in[0].normal;
        EmitVertex();
        gl_Position = explode(_in[1].position, normal);
        _out.position = vec3(gl_Position);
        _out.texCoords = _in[1].texCoords;
        _out.normal = _in[1].normal;
        EmitVertex();
        gl_Position = explode(_in[2].position, normal);
        _out.position = vec3(gl_Position);
        _out.texCoords = _in[2].texCoords;
        _out.normal = _in[2].normal;
        EmitVertex();
        EndPrimitive();
    }
    else
    {
        gl_Position = _in[0].position;
        _out.position = vec3(_in[0].position);
        _out.texCoords = _in[0].texCoords;
        _out.normal = _in[0].normal;
        _out.direction = vec3(0);
        EmitVertex();
        gl_Position = _in[1].position;
        _out.position = vec3(_in[1].position);
        _out.texCoords = _in[1].texCoords;
        _out.normal = _in[1].normal;
        _out.direction = vec3(0);
        EmitVertex();
        gl_Position = _in[2].position;
        _out.position = vec3(_in[2].position);
        _out.texCoords = _in[2].texCoords;
        _out.normal = _in[2].normal;
        _out.direction = vec3(0);
        EmitVertex();
        EndPrimitive();
    }
    
}