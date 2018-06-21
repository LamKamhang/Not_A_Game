#pragma once
#include <glad/glad.h>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "PhysicsEngine.h"
#include "Camera.h"
#include "Shader.h"

// 1/(2*sqrt(3))
#define HRrate 0.2886751345948129
#define PI 3.14159265358979323846
#define EdgeNum 6
#define CRYSTAL_SPEED 2.5f
#define CloseEnough 18.0f
#define AccelerFactor 0.003f
#define RaiusRate 3.0f  //攻击范围
#define BloodViewRate 0.25f  //过近警报
#define CloseRate 0.5f  //加速距离
#define LifeTime 40.0f //寿命

class Crystal{
private:
    GLuint VAO,VBO;
    GLfloat height,radius;
    std::vector<GLfloat>vertexs;
    
    glm::vec3 TargetPos;// move direction target position
    glm::vec3 velocity;
    glm::vec3 acceler;//self circle movement
    PhysicsEngine* physicsEngine;// physicsEngine used

    //vert movement
    glm::vec3 VertVelocity;        //垂直方向速度
	glm::vec3 accelerUp;       //方向向上的加速度
	bool isJumping;  
    
    // status
    float age;
    bool IsDead;
    int type;// good 1, bad 0
    
    bool firstupdate;
public:
    glm::vec3 Position;//position

    Crystal(PhysicsEngine* physicsEngine, glm::vec3 position=glm::vec3(0.0f), GLfloat height=2.5f,int type=0);
    GLuint getVAO(){return VAO;}
    GLuint getVBO(){return VBO;}
    GLfloat getHeight(){return height;}
    GLfloat getRadius(){return radius;}
    void die(){IsDead = 1;}
    void ageIncrease(float da){age += da;}
    float getAge(){return age;}
    bool IsOk(){return !IsDead;}
    void jump();

    void draw();
    void updatePosition(const glm::vec3 cameraPos, const GLfloat deltaTime);
};


class CrystalSystem{
private:
    std::map<int,Crystal> GoodCrystals;
    std::map<int,Crystal> BadCrystals;
    int goodCnt;
    int badCnt;
    PhysicsEngine* physicsEngine;
    Shader CryShader;
    float lastTime;
public:
    CrystalSystem(PhysicsEngine* pE)
    :CryShader("Resource/Shader/crystal.vs","Resource/Shader/crystal.fs")
    {
        goodCnt=badCnt=0;
        lastTime = 0.0f;
        GoodCrystals.clear();
        BadCrystals.clear();
        physicsEngine=pE;
    }
    // randomly generate crystal
    void generateCrystal(glm::vec3 centerPos,float areaRadius,float frequency,float goodRate,float curTime);
    
    // add a crystal
    void addCrystal(glm::vec3 position=glm::vec3(0.0f), GLfloat height=2.5f, int type=0);
    
    // update all crystal's position
    void updateAll(const glm::vec3 cameraPos, const GLfloat deltaTime);
    
    // update hero state
    void updateHeroState(const glm::vec3 &cameraPos,int &closeEnough,int &damage,int &bullet);

    // draw all crystal
    void drawAll(const glm::mat4 &projection,const glm::mat4 &view,const glm::vec3 &cameraPos,const unsigned int skyboxID,float deltaTime);
};
