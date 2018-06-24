#pragma once 
#include <iostream>
#include <vector>
#include <cstdlib>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Texture.h"

enum ParticleType{
    FLAME_LANUCHER,
    FLAME_SHELL
};

#define PARTICLE_MAX_VELOCITY 30.0f
#define PARTICLE_MIN_VELOCITY 10.0f
#define PARTICLE_MAX_LIFE 5.0f
#define PARTICLE_MIN_LIFE 0.2f

class Particle{
public:
    int type;
    glm::vec3 position;
    glm::vec3 velocity;
    float alpha;
    float age;  
    float size;
    float life;
};

class ParticleSystem{
private:
    std::vector<Particle> particles;
    unsigned int VAO,VBO;
    unsigned int texture0,texture1;
public:
    void InitFrame(int num)
    {
        srand(time(NULL));
        particles.clear();
        const int guass_n = 10;
        for(int i=0;i<num;i++){
            Particle curparticle;
            curparticle.position = glm::vec3(0.0f);
            for(int j=0;j<guass_n;j++){
                curparticle.position.x += 2.0f*float(rand())/float(RAND_MAX) - 1.0f;
                curparticle.position.z += 2.0f*float(rand())/float(RAND_MAX) - 1.0f;
            }
            curparticle.type = FLAME_LANUCHER;
            curparticle.velocity = ((PARTICLE_MAX_VELOCITY - PARTICLE_MIN_VELOCITY)*(float(rand()) / float(RAND_MAX)) + PARTICLE_MIN_VELOCITY)*glm::vec3(0.0f,1.0f,0.0f);
            curparticle.alpha = 1.0f;
            curparticle.size = 30.0f;
            curparticle.life = (PARTICLE_MAX_LIFE-PARTICLE_MIN_LIFE)*(float(rand()) / float(RAND_MAX)) + PARTICLE_MIN_LIFE;
            curparticle.age = 0.0f;

            particles.push_back(curparticle);
        }
        float vertexs[]={0.0,0.0,0.0};
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);
            glGenBuffers(1,&VBO);
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
                glBufferData(GL_ARRAY_BUFFER,3 * sizeof(float),&vertexs[0],GL_STATIC_DRAW);
                glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(GLvoid*)(0));
                glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
        
        texture0 = loadTexture("awesomeface.png","Resource/Texture");
    }

    void updateAll(const glm::vec3 &centerPos,const glm::vec3 &direction,const glm::vec3 &right, const float &radius,const float &curTime)
    {
        glm::vec3 x=glm::normalize(right);
        glm::vec3 y=glm::normalize(direction);
        glm::vec3 z=glm::cross(x,y);
        glm::mat4 mymodel=glm::mat4(glm::vec4(x,0.0f),glm::vec4(y,0.0f),glm::vec4(z,0.0f),glm::vec4(0.0f,0.0f,0.0f,1.0f));
        glm::mat4 model(1.0f);
        model = glm::translate(model,centerPos);
        model = model * mymodel;
        model = glm::scale(model,glm::vec3(radius));

        for(int i=0;i<particles.size();i++){
            particles[i].position = glm::vec3(model* glm::vec4(particles[i].position,1.0f));
            particles[i].velocity = glm::length(particles[i].velocity) * y;
            // particles[i].age = curTime;
        }
    }

    void drawAllParticle(Shader &shader,float curTime)
    {
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        shader.setInt("flameSpark",0);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture0);
        // shader.setInt("flameStart",1);

        for(int i=0;i<particles.size();i++){
            shader.setInt("type",particles[i].type);
            shader.setFloat("age",particles[i].age);
            shader.setFloat("alpha",particles[i].alpha);
            shader.setFloat("size",particles[i].size);
            shader.setFloat("lifeTime",particles[i].life);
            shader.setVec3("velocity",particles[i].velocity);
            shader.setFloat("initTime",0.0f);
            shader.setFloat("time",curTime);

            glm::mat4 model(1.0f);
            model = glm::translate(model,particles[i].position);
            shader.setMat4("model",model);
            glDrawArrays(GL_POINTS, 0, 1);
        }
        glBindVertexArray(0);
        glDisable(GL_BLEND);
    }
};
