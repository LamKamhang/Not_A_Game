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

class Gan{
private:
    Model Herogan;
    Shader ganShader;
    glm::mat4 model;
    bool IsAttacking;
    glm::vec3 modelMuzzlePos;
    glm::vec3 modelDirection;
public:
    glm::vec3 muzzlePos;
    glm::vec3 direction;

    Gan(const char* ganPath = "Resource/Model/SKgan/ SK.obj"):
    Herogan(ganPath),
    ganShader("Resource/Shader/gan.vs", "Resource/Shader/gan.fs"),
    model(0.0f),
    muzzlePos(0.0f),
    IsAttacking(false)
    {
        glm::vec3 muzzleUp(-3.284334f, 4.980214f, 0.0f);
        glm::vec3 muzzleBt(-3.284334f, 4.487482f, 0.0f);
        modelMuzzlePos = (muzzleUp + muzzleBt)/2.0f; // in model space
        modelDirection = glm::vec3(-1.0f,0.0f,0.0f); // in model space
    }

    void updateModel(Camera &camera,GLfloat deltaTime)
    {
        model=glm::mat4(1.0f);
        glm::vec3 HeroEyeFront = camera.GetEyeFront();
        glm::vec3 HeroEyeUp = camera.GetUp();
        glm::vec3 HeroEyeRight =camera.GetRight();
        glm::vec3 HeroEyePos = camera.GetPosition();

        model = glm::translate(model,2.9f * HeroEyeFront);
        model = glm::translate(model,0.6f * HeroEyeRight);
        model = glm::translate(model,-1.5f * HeroEyeUp);
        model = glm::translate(model,glm::vec3(HeroEyePos.x,HeroEyePos.y,HeroEyePos.z));
        model = glm::rotate(model,(float)PI/13.0f,HeroEyeUp);
        glm::mat4 GanRotateMatrix = glm::mat4(glm::vec4(-1.0f * HeroEyeFront,0.0f),glm::vec4(HeroEyeUp,0.0f),glm::vec4(-1.0f*HeroEyeRight,0.0f),glm::vec4(0.0f,0.0f,0.0f,1.0f));
        model = model * GanRotateMatrix;
        model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
        
        // update muzzlePos in world space
        glm::vec4 tmp = model * glm::vec4(modelMuzzlePos,1.0f);
        muzzlePos = glm::vec3(tmp.x,tmp.y,tmp.z)/tmp.w;
        // update gan's direction in world space
        tmp = model * glm::vec4(modelDirection,1.0f);
        glm::vec4 origin = model * glm::vec4(0.0f,0.0f,0.0f,1.0f);
        direction = glm::normalize( glm::vec3(tmp.x,tmp.y,tmp.z) / tmp.w - glm::vec3(origin.x,origin.y,origin.z) / origin.w);
    }

    void draw(const glm::mat4 &projection,const glm::mat4 &view,const glm::vec3 &cameraPos)
    {
        ganShader.use();
			ganShader.setVec3("viewPos", cameraPos);
			ganShader.setMat4("projection", projection);
			ganShader.setMat4("view", view);
			ganShader.setMat4("model", model);
			Herogan.Draw(ganShader);
		glUseProgram(0);
    }
};
