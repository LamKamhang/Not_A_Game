#pragma once 
#include <iostream>
#include <vector>
#include <cstdlib>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Texture.h"
#define PI 3.141592653589793238463
#define PARTICLE_GENERATED_PER_DECOND 10000.0f
#define MIN_DELTATIME 0.016f
#define MaxParticles 20000
#define MaxTextureNum 100
float randfloat(){return float(rand()%1000)/1000.0f;}

static const GLfloat g_vertex_buffer_data[]={
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f
};

struct Particle{
    glm::vec3 pos,speed;
    float r,g,b,a;
    float size,angle,weight,speedrate;
    float life;
    float cameradistance;//for sort
    bool operator < (const Particle& that) const
    {
        //far particles drawn first
        return this->cameradistance > that.cameradistance;
    }
    Particle():life(-1.0f){}
};

class Particlesystem{
private:
    GLuint VAO;
    GLuint billboard_vertex_buffer;
    GLuint particles_position_buffer;
    GLuint particles_color_buffer;
    
    GLfloat g_particule_position_size_data[MaxParticles];
    GLfloat g_particule_color_data[MaxParticles];

    Particle ParticlesContainer[MaxParticles];
    int LastUsedParticle;
    int ParticlesCount; // #alive particles

    // unsigned int texture[MaxTextureNum];
    // int currentTextureNum;
    unsigned int texture0;

    // Finds a Particle in ParticlesContainer which isn't used yet.
    int FindUnusedParticle(){
        for(int i=LastUsedParticle; i<MaxParticles; i++){
            if (ParticlesContainer[i].life < 0){
                LastUsedParticle = i;
                return i;
            }
        }
        for(int i=0; i<LastUsedParticle; i++){
            if (ParticlesContainer[i].life < 0){
                LastUsedParticle = i;
                return i;
            }
        }
        return 0; // All particles are taken, override the first one
    }
public:

    Particlesystem(const char* texName = "awesomeface.png"){
        LastUsedParticle = 0;
        texture0 = loadTexture(texName, "Resource/Texture");
        
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);
        //vertex
        glGenBuffers(1, &billboard_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        //position
        glGenBuffers(1, &particles_position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        //colors
        glGenBuffers(1, &particles_color_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        
        glBindVertexArray(0);
    }

    void updateBuffer()
    {
        glBindVertexArray(VAO);

        //vertex
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        
        //position
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);
        
        //colors
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_color_data);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glVertexAttribPointer(
        0, // attribute. No particular reason for 0, but must match the layout in the shader.
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
        );

        // 2nd attribute buffer : positions of particles' centers
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glVertexAttribPointer(
        1, // attribute. No particular reason for 1, but must match the layout in the shader.
        4, // size : x + y + z + size => 4
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
        );

        // 3rd attribute buffer : particles' colors
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glVertexAttribPointer(
        2, // attribute. No particular reason for 1, but must match the layout in the shader.
        4, // size : r + g + b + a => 4
        GL_FLOAT, // type
        GL_FALSE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0, // stride
        (void*)0 // array buffer offset
        );
        
        glBindVertexArray(0);
    }
    
    void sortParticles(){
        std::sort(&ParticlesContainer[0],&ParticlesContainer[MaxParticles]);
    }

    void updateParticlesFountain(GLfloat deltaTime,const glm::vec3 &CameraPosition,const glm::vec3 &startPos)
    {
        // add new particles
        int newparticles = (int)(0.2f * deltaTime * PARTICLE_GENERATED_PER_DECOND);
        if (newparticles > (int)(MIN_DELTATIME * PARTICLE_GENERATED_PER_DECOND))
            newparticles = (int)(MIN_DELTATIME * PARTICLE_GENERATED_PER_DECOND);
        
        for(int i = 0;i < newparticles; i++){
            int cur = FindUnusedParticle();
            Particle& p = ParticlesContainer[cur];

            p.life = 2.0f;
            p.speedrate = 5.0f;
            p.size = 0.2f;
            p.weight = 1.0f;
            p.angle = 2.0f * PI * randfloat();
            p.a = 0.5f;p.r = randfloat();p.g = randfloat();p.b = randfloat();
            
            p.pos = startPos;
            p.cameradistance = glm::length( p.pos - CameraPosition );
            // 喷泉
            p.speed = p.speedrate * glm::normalize(glm::vec3(sin(p.angle),3.0f,cos(p.angle)));
        }

        // sort particles
        sortParticles();

        // update existed particles
        ParticlesCount = 0;
        for(int i=0; i< MaxParticles; i++){
            Particle& p = ParticlesContainer[i]; // shortcut
            if(p.life > 0.0f){
                // Decrease life
                p.life -= deltaTime;
                if (p.life > 0.0f){
                    // Simulate simple physics : gravity only, no collisions
                    p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)deltaTime * 0.5f;
                    p.pos += p.speed * (float)deltaTime;
                    p.cameradistance = glm::length( p.pos - CameraPosition );
                    //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)deltaTime;

                    // Fill the GPU buffer
                    g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
                    g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
                    g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;

                    g_particule_position_size_data[4*ParticlesCount+3] = p.size;

                    g_particule_color_data[4*ParticlesCount+0] = p.r;
                    g_particule_color_data[4*ParticlesCount+1] = p.g;
                    g_particule_color_data[4*ParticlesCount+2] = p.b;
                    g_particule_color_data[4*ParticlesCount+3] = p.a;

                }else{
                    // Particles that just died will be put at the end of the buffer in SortParticles();
                    p.cameradistance = -10.0f;
                }
                ParticlesCount++;
            }
        }
    }

    void updateParticlesFlame(bool IsAttacking,float deltaTime,const glm::vec3 &BulletPos,const glm::vec3 &BulletDir,const glm::vec3 &CameraPos)
    {
        // add new particles
        int newparticles = (int)(deltaTime * PARTICLE_GENERATED_PER_DECOND);
        if (newparticles > (int)(MIN_DELTATIME * PARTICLE_GENERATED_PER_DECOND))
            newparticles = (int)(MIN_DELTATIME * PARTICLE_GENERATED_PER_DECOND);
        if(!IsAttacking)newparticles = 0;
        for(int i = 0;i < newparticles; i++){
            int cur = FindUnusedParticle();
            Particle& p = ParticlesContainer[cur];

            p.life = 1.0f;
            p.speedrate = 1.0f;
            p.size = 0.2f;
            p.weight = 1.0f;
            p.angle = 2.0f * PI * randfloat();
            // flame color
            p.r = 0.8f;p.g = 0.8f;p.b = 0.1f;p.a = 0.5f;
            
            p.pos = BulletPos;
            p.cameradistance = glm::length( p.pos - CameraPos );
            glm::vec3 n = glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f),BulletDir));
            glm::vec3 m = glm::normalize(glm::cross(BulletDir,n));
            float alpha = 0.2f * randfloat();
            p.speed = p.speedrate * ( alpha * (cos(p.angle) * n + sin(p.angle) * m) + glm::normalize(BulletDir));
        }

        // sort particles
        sortParticles();

        // update existed particles
        ParticlesCount = 0;
        for(int i=0; i<MaxParticles; i++){
            Particle& p = ParticlesContainer[i]; // shortcut
            if(p.life > 0.0f){
                // Decrease life
                p.life -= deltaTime;
                if (p.life > 0.0f){
                    p.size -= 0.3f * deltaTime;
                    if(p.size < 0.0f)p.size=0.0001f;
                    p.g -= deltaTime;
                    if(p.g<0.0f)p.g = 0.0f;
                    
                    p.pos += p.speed * (float)deltaTime;
                    p.cameradistance = glm::length( p.pos - CameraPos );

                    // Fill the GPU buffer
                    g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
                    g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
                    g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;

                    g_particule_position_size_data[4*ParticlesCount+3] = p.size;

                    g_particule_color_data[4*ParticlesCount+0] = p.r;
                    g_particule_color_data[4*ParticlesCount+1] = p.g;
                    g_particule_color_data[4*ParticlesCount+2] = p.b;
                    g_particule_color_data[4*ParticlesCount+3] = p.a;
                }else{
                    // Particles that just died will be put at the end of the buffer in SortParticles();
                    p.cameradistance = -10.0f;
                }
                ParticlesCount++;
            }
        }
    }

    void updateParticlesOpenFire(bool IsAttacking,float deltaTime,float curTime,const glm::vec3 &ganPos,const glm::vec3 &ganDir,const glm::vec3 &CameraPos)
    {
        static float startTime = -1.0f;
        const float MaxTime = 0.2f;

        // add new particles
        int newparticles = (int)(2.0f * deltaTime * PARTICLE_GENERATED_PER_DECOND);
        if (newparticles > (int)(MIN_DELTATIME * PARTICLE_GENERATED_PER_DECOND))
            newparticles = (int)(MIN_DELTATIME * PARTICLE_GENERATED_PER_DECOND);
        
        if(!IsAttacking){newparticles = 0;startTime = -1.0f;}
        else if(startTime == -1.0f)startTime = curTime;
        if(curTime - startTime > MaxTime)newparticles = 0;

        for(int i = 0;i < newparticles; i++){
            int cur = FindUnusedParticle();
            Particle& p = ParticlesContainer[cur];

            p.life = 1.0f;
            p.speedrate = 10.0f;
            p.size = 0.2f;
            p.weight = 1.0f;
            p.angle = 2.0f * PI * randfloat();
            // flame color
            p.r = 0.8f;p.g = 0.5f;p.b = 0.1f;p.a = 0.5f;
            
            p.pos = ganPos;
            p.cameradistance = glm::length( p.pos - CameraPos );
            glm::vec3 n = glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f),ganDir));
            glm::vec3 m = glm::normalize(glm::cross(ganDir,n));
            float alpha = 0.48f * randfloat();
            p.speed = p.speedrate * ( alpha * (cos(p.angle) * n + sin(p.angle) * m) + glm::normalize(ganDir));
        }

        // sort particles
        sortParticles();

        // update existed particles
        ParticlesCount = 0;
        for(int i=0; i<MaxParticles; i++){
            Particle& p = ParticlesContainer[i]; // shortcut
            if(p.life > 0.0f){
                // Decrease life
                p.life -= deltaTime;
                if (p.life > 0.0f){
                    p.size -= 0.5f * deltaTime;
                    if(p.size < 0.0f)p.size=0.0001f;
                    p.g -= deltaTime;
                    if(p.g<0.0f)p.g = 0.0f;
                    
                    // p.speed += glm::vec3(0.0f,4.9f,0.0f)* deltaTime;
                    p.pos += p.speed * (float)deltaTime;
                    p.cameradistance = glm::length( p.pos - CameraPos );

                    // Fill the GPU buffer
                    g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
                    g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
                    g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;

                    g_particule_position_size_data[4*ParticlesCount+3] = p.size;

                    g_particule_color_data[4*ParticlesCount+0] = p.r;
                    g_particule_color_data[4*ParticlesCount+1] = p.g;
                    g_particule_color_data[4*ParticlesCount+2] = p.b;
                    g_particule_color_data[4*ParticlesCount+3] = p.a;
                }else{
                    // Particles that just died will be put at the end of the buffer in SortParticles();
                    p.cameradistance = -10.0f;
                }
                ParticlesCount++;
            }
        }
    }

    void drawParticles(Shader &shader)
    {
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(VAO);
        
        shader.setInt("texture0",0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        
        // The first parameter is the attribute buffer we're talking about.
        // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
        glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
        glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
        glVertexAttribDivisor(2, 1); // color : one per quad -> 1

        // Draw the particules
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);
        
        glBindVertexArray(0);
        glDisable(GL_BLEND);
    }
};
