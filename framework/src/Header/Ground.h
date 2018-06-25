#pragma once
#include <glad/glad.h>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "PhysicsEngine.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

#define GROUND_SIZE 500.0f
#define YGap -0.01f
#define GRASSNUM 1000
static const float groundVertexs[]={
    1.0,0.0,-1.0, 0.0,1.0,0.0, 1.0,1.0,
    -1.0,0.0,-1.0, 0.0,1.0,0.0, 0.0,1.0,
    -1.0,0.0,1.0, 0.0,1.0,0.0, 0.0,0.0,
    1.0,0.0,-1.0, 0.0,1.0,0.0, 1.0,1.0,
    -1.0,0.0,1.0, 0.0,1.0,0.0, 0.0,0.0,
    1.0,0.0,1.0, 0.0,1.0,0.0, 1.0,0.0
};

static const float g_vertex_buffer_data[]={
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f
};

struct Grass{
    glm::vec3 position;
    float size;
    float cameradistance;
    bool operator < (const Grass& that) const{
        return this->cameradistance > that.cameradistance;
    }
};

class Ground{
private:
    unsigned int groundVAO,groundVBO;
    unsigned int grassVAO,grass_position_buffer,billboard_vertex_buffer;
    unsigned int texture0;
    Grass grassContainer[GRASSNUM];
    GLfloat grassPosition[GRASSNUM];
    bool Initialed;
public:
    Ground(const char *texName="ground.jpeg")
    {
        Initialed = 0;
        texture0 = loadTexture(texName,"Resource/Texture");
        glGenVertexArrays(1,&groundVAO);
        glBindVertexArray(groundVAO);
            glGenBuffers(1,&groundVBO);
            glBindBuffer(GL_ARRAY_BUFFER,groundVBO);
                glBufferData(GL_ARRAY_BUFFER,sizeof(groundVertexs),&groundVertexs[0],GL_STATIC_DRAW);
                glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(0));
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(3*sizeof(float)));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(6*sizeof(float)));
                glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);

        glGenVertexArrays(1,&grassVAO);
        glBindVertexArray(grassVAO);
        //vertex
        glGenBuffers(1, &billboard_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        //position
        glGenBuffers(1, &grass_position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, grass_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, GRASSNUM * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        
        glBindVertexArray(0);
    }
    
    void setGroundCollision(Camera &camera){
        camera.SetinnerBound(glm::vec3(-GROUND_SIZE,-10.0f,-GROUND_SIZE),glm::vec3(GROUND_SIZE,YGap,GROUND_SIZE));
    }

    void updateGrass(const glm::vec3 &cameraPos)
    {
        if(!Initialed){
            for(int i=0;i<GRASSNUM;i++)
            {
                Grass &g = grassContainer[i];
            }
        }
        
    }
    
    void draw(Shader &shader)
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model,glm::vec3(0.0f,YGap,0.0f));
        model = glm::scale(model,glm::vec3(GROUND_SIZE));
        glBindVertexArray(groundVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture0);
            shader.setInt("texture0",0);
            shader.setMat4("model",model);
            glDrawArrays(GL_TRIANGLES,0,6);
        glBindVertexArray(0);
    }
};
