#include "Header/Crystal.h"

Crystal::Crystal(PhysicsEngine* physicsEngine, glm::vec3 position, GLfloat height,int type)
:Position(position),TargetPos(position),velocity(0.0f),acceler(0.0f)
{
    IsDead = 0;
    firstupdate = 1;
    this->type=type;
    this->physicsEngine = physicsEngine;
    this->height=height;
    this->radius=height*HRrate;
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

void Crystal::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,vertexs.size()/8);
    glBindVertexArray(0);
}

void Crystal::updatePosition(const glm::vec3 cameraPos, const GLfloat deltaTime)
{
    glm::vec3 direc=cameraPos - Position;
    float distance = glm::length(glm::vec3(direc.x,0.0f,direc.z));
    if(distance < 3.0f * radius){
        if(!type)velocity = glm::normalize( glm::vec3(direc.x,0.0f,direc.z) ) * -1.5f * speed * deltaTime;
        else velocity = glm::normalize( glm::vec3(direc.x,0.0f,direc.z) ) * speed * deltaTime;
    }
    else if(distance < 0.5f * CloseEnough){
        if(!type)velocity = glm::normalize( glm::vec3(direc.x,0.0f,direc.z) ) * 1.1f * speed * deltaTime;
        else velocity = glm::normalize( glm::vec3(direc.x,0.0f,direc.z) ) * speed * deltaTime;
    }
    else if(distance < CloseEnough){
        velocity = glm::normalize( glm::vec3(direc.x,0.0f,direc.z) ) * speed * deltaTime;
    }
    else{
        if(firstupdate)velocity = glm::vec3(0.0f,0.0f,-1.0f) * speed * deltaTime;
        else{
            acceler = glm::normalize(glm::vec3(velocity.z,0.0f,-velocity.x)) * AccelerFactor * speed * deltaTime;
            velocity += acceler;
            velocity=glm::normalize(velocity) * speed * deltaTime;
        }
    }
    Position += velocity;
    TargetPos = Position + 10.0f * velocity;

    physicsEngine->outCollisionTest(Position, TargetPos);
    physicsEngine->inCollisionTest(Position, TargetPos);
    
    if(firstupdate)firstupdate=0;
}



//###################### CrystalSystem ######################

void CrystalSystem::addCrystal(glm::vec3 position, GLfloat height, int type)
{
    if(type)GoodCrystals.insert(std::make_pair(goodCnt++,Crystal(physicsEngine,position,height,1)));
    else BadCrystals.insert(std::make_pair(badCnt++,Crystal(physicsEngine,position,height,0)));
}

void CrystalSystem::updateAll(const glm::vec3 cameraPos, const GLfloat deltaTime)
{
    std::map<int,Crystal>::iterator p;
    for(p=GoodCrystals.begin();p!=GoodCrystals.end();p++)p->second.updatePosition(cameraPos,deltaTime);
    for(p=BadCrystals.begin();p!=BadCrystals.end();p++)p->second.updatePosition(cameraPos,deltaTime);
}

void CrystalSystem::drawAll(const glm::mat4 &projection,const glm::mat4 &view,const glm::vec3 &cameraPos,const unsigned int skyboxID,float deltaTime)
{
    std::map<int,Crystal>::iterator p;
    CryShader.use();
        CryShader.setMat4("projection",projection);
        CryShader.setMat4("view",view);
        CryShader.setVec3("cameraPos",cameraPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
        CryShader.setInt("skybox",0);
        CryShader.setVec3("color",glm::vec3(0.0f,0.0f,0.5f));//good color blue
        for(p=GoodCrystals.begin();p!=GoodCrystals.end();p++)if(p->second.IsOk()){
            
            glm::mat4 model=glm::mat4(1.0f);
            model=glm::translate(model,p->second.Position);
            CryShader.setMat4("model", model);
            p->second.draw();//draw
        }
        CryShader.setVec3("color",glm::vec3(0.8f,0.0f,0.0f));//good color blue
        for(p=BadCrystals.begin();p!=BadCrystals.end();p++)if(p->second.IsOk()){
            glm::mat4 model=glm::mat4(1.0f);
            model=glm::translate(model,p->second.Position);
            CryShader.setMat4("model", model);
            p->second.draw();//draw
        }
    glUseProgram(0);
}

void CrystalSystem::updateHeroState(const glm::vec3 &cameraPos,int &closeEnough,int &damage,int &bullet)
{
    closeEnough = 0;
    std::map<int,Crystal>::iterator p;
    for(p=BadCrystals.begin();p!=BadCrystals.end();p++){
        glm::vec3 direc = glm::vec3(cameraPos.x,cameraPos.y-HeroHeight,cameraPos.z) - p->second.Position;
        if(glm::length(direc) < CloseEnough)
            closeEnough = 1;// enable blood view
        if(glm::length(direc) < 2.0f * p->second.getRadius())
            damage++;// be attacked and loss blood
    }
    for(p=GoodCrystals.begin();p!=GoodCrystals.end();p++){
        glm::vec3 direc=glm::vec3(cameraPos.x,cameraPos.y-HeroHeight,cameraPos.z) - p->second.Position;
        if(glm::length(direc) < 2.0f * p->second.getRadius()){
            bullet++;// get bullets
            p->second.die();
        }
    }
}