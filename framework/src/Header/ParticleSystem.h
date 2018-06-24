#pragma once 
#include <iostream>
#include <vector>
#include <cstdlib>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

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
    float MaxAge;
};

class ParticleSystem{
private:
    std::vector<Particle> particles;
    unsigned int VAO,VBO;
public:
    void InitFrame(const glm::vec3 &centerPosition, const glm::vec3 &direction, int num)
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
            curparticle.velocity = ((PARTICLE_MAX_VELOCITY - PARTICLE_MIN_VELOCITY)*(float(rand()) / float(RAND_MAX)) + PARTICLE_MIN_VELOCITY)*glm::normalize(direction);
            curparticle.alpha = 1.0f;
            curparticle.size = 1.0f;
            curparticle.MaxAge = (PARTICLE_MAX_LIFE-PARTICLE_MIN_LIFE)*(float(rand()) / float(RAND_MAX)) + PARTICLE_MIN_LIFE;
            curparticle.age = 0.0f;

            particles.push_back(curparticle);
        }
    }

    void drawAllParticle(Shader &shader)
    {

    }
};