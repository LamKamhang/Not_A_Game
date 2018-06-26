#pragma once
#include <glad/glad.h>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

#define GROUND_SIZE 500.0f
#define YGap -0.01f
#define GRASSNUM 1000
float myrandfloat(){return float(rand()%1000)/1000.0f;}

static const float groundVertexs[]={
    1.0,0.0,-1.0, 0.0,1.0,0.0, 1.0,1.0,
    -1.0,0.0,-1.0, 0.0,1.0,0.0, 0.0,1.0,
    -1.0,0.0,1.0, 0.0,1.0,0.0, 0.0,0.0,
    1.0,0.0,-1.0, 0.0,1.0,0.0, 1.0,1.0,
    -1.0,0.0,1.0, 0.0,1.0,0.0, 0.0,0.0,
    1.0,0.0,1.0, 0.0,1.0,0.0, 1.0,0.0
};

static const float grass_vertexs[]={
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
    unsigned int texture0,texture1;
    Grass grassContainer[GRASSNUM];
    GLfloat grassPosition[4*GRASSNUM];
    bool Initialed;
    Shader groundShader;
    Shader grassShader;
    glm::vec3 roombound1,roombound2;
public:
    Ground(const char *groundTex="ground.jpeg",const char *grassTex="grass.png"):
    groundShader("Resource/Shader/ground.vs","Resource/Shader/ground.fs"),
    grassShader("Resource/Shader/grass.vs","Resource/Shader/grass.fs")
    {
        Initialed = 0;
        texture0 = loadTexture(groundTex,"Resource/Texture");
        texture1 = loadTexture(grassTex,"Resource/Texture");
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(grass_vertexs), grass_vertexs, GL_STATIC_DRAW);
        //position
        glGenBuffers(1, &grass_position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, grass_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, GRASSNUM * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        
        glBindVertexArray(0);
    }
    
    void setGroundCollision(Camera &camera){
        camera.SetinnerBound(glm::vec3(-GROUND_SIZE,-10.0f,-GROUND_SIZE),glm::vec3(GROUND_SIZE,YGap,GROUND_SIZE));
    }

    void setRoomBound(const glm::vec3 &roombound1,const glm::vec3 &roombound2)
    {
        this->roombound1=roombound1;
        this->roombound2=roombound2;
    }

    void updateGrass(const glm::vec3 &cameraPos)
    {
        if(!Initialed){
            for(int i=0;i<GRASSNUM;i++){
                Grass &g = grassContainer[i];
                g.size = 4.0f * myrandfloat() + 5.0f;
                g.position = GROUND_SIZE * glm::vec3(2.0f* myrandfloat()-1.0f, 0.0f, 2.0f*myrandfloat()-1.0f) + glm::vec3(0.0f,YGap + 0.5f * g.size,0.0f);
                bool inside = (g.position.x>roombound1.x&&g.position.x<roombound2.x)&&(g.position.z>roombound1.z&&g.position.z<roombound2.z);
                while(inside){
                    g.position = GROUND_SIZE * glm::vec3(2.0f* myrandfloat()-1.0f, 0.0f, 2.0f*myrandfloat()-1.0f) + glm::vec3(0.0f,YGap + 0.5f * g.size,0.0f);    
                    inside = (g.position.x>roombound1.x&&g.position.x<roombound2.x)&&(g.position.z>roombound1.z&&g.position.z<roombound2.z);
                }
                g.cameradistance = glm::length(g.position - cameraPos);
            }
            Initialed = 1;
        }
        else{
            for(int i=0;i<GRASSNUM;i++){
                Grass &g = grassContainer[i];
                g.cameradistance = glm::length(g.position - cameraPos);
            }
        }

        std::sort(&grassContainer[0],&grassContainer[GRASSNUM]);

        for(int i = 0;i < GRASSNUM;i++){
            Grass &g = grassContainer[i];
            grassPosition[i*4 + 0] = g.position.x;
            grassPosition[i*4 + 1] = g.position.y;
            grassPosition[i*4 + 2] = g.position.z;
            grassPosition[i*4 + 3] = g.size;
        }
    }

    void updateBuffer()
    {
        glBindVertexArray(grassVAO);
        //vertex
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(grass_vertexs), grass_vertexs, GL_STATIC_DRAW);
        
        //position
        glBindBuffer(GL_ARRAY_BUFFER, grass_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, GRASSNUM * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, GRASSNUM * sizeof(GLfloat) * 4, grassPosition);
        
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
        glBindBuffer(GL_ARRAY_BUFFER, grass_position_buffer);
        glVertexAttribPointer(
        1, // attribute. No particular reason for 1, but must match the layout in the shader.
        4, // size : x + y + z + size => 4
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
        );
        
        glBindVertexArray(0);
    }

    void draw(const glm::mat4&projection,const glm::mat4&view,Camera &camera)
    {
        // draw ground !!
        groundShader.use();
        groundShader.setMat4("projection",projection);
		groundShader.setMat4("view",view);
        glm::mat4 model(1.0f);
        model = glm::translate(model,glm::vec3(0.0f,YGap,0.0f));
        model = glm::scale(model,glm::vec3(GROUND_SIZE));
        groundShader.setMat4("model",model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        groundShader.setInt("texture0",0);
        glBindVertexArray(groundVAO);
            glDrawArrays(GL_TRIANGLES,0,6);
        glBindVertexArray(0);
        
        // draw grass !!
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        grassShader.use();
            grassShader.setMat4("projection",projection);
            grassShader.setMat4("view",view);
            grassShader.setInt("texture0",0);
            grassShader.setVec3("eyeFront",camera.GetEyeFront());
			grassShader.setVec3("eyeUp",camera.GetUp());
			grassShader.setVec3("eyeRight",camera.GetRight());
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glBindVertexArray(grassVAO);
                // The first parameter is the attribute buffer we're talking about.
                // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
                glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
                glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
                // Draw grass
                glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, GRASSNUM);
            glBindVertexArray(0);
        glDisable(GL_BLEND);
    }
};
