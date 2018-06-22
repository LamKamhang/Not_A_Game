#pragma once
#include <glad/glad.h>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "PhysicsEngine.h"
#include "Shader.h"

#define HeroHeight 2.5f
#define HRrate 0.2886751345948129
#define PI 3.14159265358979323846
#define EdgeNum 6
#define BULLET_SPEED 45.0f

class Bullet{
private:
    glm::vec3 Position;
    glm::vec3 TargetPos;
    std::vector<float> vertexs;
    
    glm::vec3 velocity;
    PhysicsEngine* physicsEngine;// physicsEngine used

    float height;
    float radius;

    float lastTime;

    unsigned int bulletVAO,bulletVBO;
    Shader shader;
public:
    glm::vec3 startPos;
    glm::vec3 direction;
    
    bool IsAttacking;
    bool Hitted;

    Bullet(PhysicsEngine* physicsEngine):
    shader("Resource/Shader/bullet.vs","Resource/Shader/bullet.fs"),
    IsAttacking(false),
    startPos(glm::vec3(0.0f)),
    Position(glm::vec3(0.0f)),
    TargetPos(glm::vec3(0.0f)),
    direction(glm::vec3(0.0f,1.0f,0.0f)),
    velocity(glm::vec3(0.0f))
    {
        Hitted = 0;
        this->physicsEngine = physicsEngine;
        lastTime=0.0f;
        height=0.3f;
        radius=height*HRrate;

        vertexs.clear();
        GLfloat theta1=0.0f,theta2=0.0f;
        // create crystal vertexs
        for(int i=0;i<EdgeNum;i++){
            theta1=(float)i*2.0f*PI/EdgeNum;
            theta2=theta1 + 2.0f*PI/EdgeNum;
            float x1=radius*cos(theta1);
            float z1=-radius*sin(theta1);
            float x2=radius*cos(theta2);
            float z2=-radius*sin(theta2);
            glm::vec3 v1(x1,-height/2.0,z1);
            glm::vec3 v2(x2,-height/2.0,z2);
            glm::vec3 cross=glm::normalize(glm::cross(v1,v2));
            
            vertexs.push_back(0.0f);vertexs.push_back(height);vertexs.push_back(0.0f);
            vertexs.push_back(cross.x);vertexs.push_back(cross.y);vertexs.push_back(cross.z);
            vertexs.push_back(1.0f);vertexs.push_back(1.0f);

            vertexs.push_back(x1);vertexs.push_back(height/2.0);vertexs.push_back(z1);
            vertexs.push_back(cross.x);vertexs.push_back(cross.y);vertexs.push_back(cross.z);
            vertexs.push_back(float(i)/EdgeNum);vertexs.push_back(1.0f-float(i)/EdgeNum);

            vertexs.push_back(x2);vertexs.push_back(height/2.0);vertexs.push_back(z2);
            vertexs.push_back(cross.x);vertexs.push_back(cross.y);vertexs.push_back(cross.z);
            vertexs.push_back(float(i+1)/EdgeNum);vertexs.push_back(1.0f-float(i+1)/EdgeNum);
            
            glm::vec3 v3(x2,height/2.0,z2);
            glm::vec3 v4(x1,height/2.0,z1);
            glm::vec3 cross1=glm::normalize(glm::cross(v3,v4));

            vertexs.push_back(0.0f);vertexs.push_back(0.0f);vertexs.push_back(0.0f);
            vertexs.push_back(cross1.x);vertexs.push_back(cross1.y);vertexs.push_back(cross1.z);
            vertexs.push_back(1.0f);vertexs.push_back(1.0f);

            vertexs.push_back(x2);vertexs.push_back(height/2.0);vertexs.push_back(z2);
            vertexs.push_back(cross1.x);vertexs.push_back(cross1.y);vertexs.push_back(cross1.z);
            vertexs.push_back(float(i+1)/EdgeNum);vertexs.push_back(1.0f-float(i+1)/EdgeNum);

            vertexs.push_back(x1);vertexs.push_back(height/2.0);vertexs.push_back(z1);
            vertexs.push_back(cross1.x);vertexs.push_back(cross1.y);vertexs.push_back(cross1.z);
            vertexs.push_back(float(i)/EdgeNum);vertexs.push_back(1.0f-float(i)/EdgeNum);
        }
        // set VAO
        glGenVertexArrays(1,&bulletVAO);
        glBindVertexArray(bulletVAO);
            glGenBuffers(1,&bulletVBO);
            glBindBuffer(GL_ARRAY_BUFFER,bulletVBO);
                glBufferData(GL_ARRAY_BUFFER,vertexs.size() * sizeof(float),&vertexs[0],GL_STATIC_DRAW);
                glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(0));
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(3*sizeof(float)));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(GLvoid*)(6*sizeof(float)));
                glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }
    
    glm::vec3 getPosition(){
        return Position;
    }

    bool IsAttack(){
        return IsAttacking;
    }

    void setPhysicsEngine(PhysicsEngine* pE){
        physicsEngine = pE;
    }

    void SetStartPos(const glm::vec3 spos){
        startPos = glm::vec3(spos.x, spos.y, spos.z) + direction;
    }
    void SetDirection(const glm::vec3 direc){
        direction = glm::normalize(direc);
    }
    void Attack(){
        IsAttacking=true;
        Position = startPos;
        Hitted = 0;
    }
    void ShutDown(){
        IsAttacking=false;
    }
    void updatePosition(GLfloat deltaTime,GLfloat curTime)
    {
        if(!IsAttacking){
            lastTime = curTime;
            Hitted = 0;
        }
        if(IsAttacking){
            velocity = glm::normalize(direction) * BULLET_SPEED * deltaTime;
            bool IsHit = false;
            
            Position += velocity;
            TargetPos = Position + 50.0f * velocity;

            IsHit = physicsEngine->outCollisionTest(Position, TargetPos);
            IsHit = physicsEngine->inCollisionTest(Position, TargetPos, height/2.0f);
            
            if( IsHit || curTime - lastTime > 2.0f ){
                IsAttacking = false;
                Hitted = true;
            }
        }
    }
    void draw(const glm::mat4 &projection,const glm::mat4 &view,const glm::vec3 &cameraPos,const unsigned int skyboxID,float deltaTime)
    {
        if(IsAttacking){
            shader.use();
                shader.setMat4("projection",projection);
                shader.setMat4("view",view);
                shader.setVec3("cameraPos",cameraPos);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
                shader.setInt("skybox",0);
                shader.setVec3("color",glm::vec3(0.8f,0.8f,0.0f));//good color blue
                glm::mat4 model(1.0f);
                model = glm::translate(model,Position);
                model = glm::rotate(model,(float)PI,glm::vec3(direction.x/2.0f,(direction.y+1.0f)/2.0f,direction.z/2.0f));
                shader.setMat4("model",model);
                glBindVertexArray(bulletVAO);
                    glDrawArrays(GL_TRIANGLES,0,vertexs.size()/8);
                glBindVertexArray(0);
            glUseProgram(0);
        }
    }
    
};