#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "PhysicsEngine.h"

// 1/(2*sqrt(3))
#define HRrate 0.2886751345948129
#define PI 3.14159265358979323846
#define EdgeNum 6
#define speed 2.5f

class Crystal{
private:
    GLuint VAO,VBO;
    GLfloat height,radius;
    std::vector<GLfloat>vertexs;
    
    glm::vec3 TargetPos;
    glm::vec3 velocity;
    
public:
    glm::vec3 Position;

    Crystal(glm::vec3 position=glm::vec3(0.0f), GLfloat height=2.5f):Position(position),TargetPos(position),velocity(0.0f)
    {
        this->height=height;
        this->radius=height*HRrate;
        vertexs.clear();
        GLfloat theta1=0.0f,theta2=0.0f;
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
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);
            glGenBuffers(1,&VBO);
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
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
    GLuint getVAO(){return VAO;}
    GLuint getVBO(){return VBO;}
    GLfloat getHeight(){return height;}
    GLfloat getRadius(){return radius;}
    
    void draw()
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,vertexs.size()/8);
        glBindVertexArray(0);
    }

    void updatePosition(PhysicsEngine* physicsEngine, glm::vec3 cameraPos, GLfloat deltaTime)
    {
        glm::vec3 direc=cameraPos - Position;
        velocity = glm::normalize( glm::vec3(direc.x,0.0f,direc.z) ) * speed * deltaTime;
        Position += velocity;
        TargetPos = Position + 10.0f * velocity;

        physicsEngine->outCollisionTest(Position, TargetPos);
		physicsEngine->inCollisionTest(Position, TargetPos);
    }
};
