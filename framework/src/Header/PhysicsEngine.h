#pragma once 
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util.h"

#define GravityAcceler -9.8f  

#define BoundaryGap 0.2f          //碰撞间距
#define JumpFactor 2.0f          //跳起速度系数
#define GravityFactor 3.5f       //下落速度系数

struct dot {  
    float x;  
    float y;  
    dot(float _x, float _y) :x(_x), y(_y) { }  
};  

class PhysicsEngine {
public:  
    PhysicsEngine();  
    ~PhysicsEngine();  

    //设置空间外部边缘  
    void setSceneOuterBoundary(float x1, float z1, float x2, float z2);
    //外部碰撞检测
    bool outCollisionTest(glm::vec3 & cameraPos, glm::vec3 & targetPos);
    //设置空间内部边缘
    void setSceneInnerBoundary(float x1, float y1, float z1, float x2, float y2, float z2);
    //内部碰撞检测
    bool inCollisionTest(glm::vec3 & cameraPos, glm::vec3 & targetPos,float HeroHeight);

    //每帧绘制的时候更新摄像机垂直方向移动  
    void updateCameraVertMovement(glm::vec3 & cameraPos, glm::vec3 & targetPos, glm::vec3 & velocity, glm::vec3 &accelerUp, bool &isJumping, const float HeroHeight, GLfloat deltaTime);

private:  
    //空间内部边缘碰撞检测（考虑高度）  
    bool inCollisionTestWithHeight(float x1, float y1, float z1, float x2, float y2, float z2, glm::vec3 & cameraPos, glm::vec3 & targetPos, float HeroHeight);
    //空间内部边缘碰撞检测（不考虑高度，即XZ平面）  
    bool inCollisionTestXZ(float x1, float z1, float x2, float z2, glm::vec3 & cameraPos, glm::vec3 & targetPos);  
    //空间外部边缘碰撞检测  
    bool outCollisionTestXZ(float x1, float z1, float x2, float z2, glm::vec3 & cameraPos, glm::vec3 & targetPos);  
  
    glm::vec3 gravity;         //重力加速度
  
    glm::vec4 outerBoundary;  
    std::vector<glm::vec3> innerBoundaryMin;    //碰撞器小的x/y/z坐标
    std::vector<glm::vec3> innerBoundaryMax;    //碰撞器大的x/y/z坐标
};  
