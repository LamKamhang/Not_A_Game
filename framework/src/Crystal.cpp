#include "Header/Crystal.h"
#include <cstdlib>

Crystal::Crystal(PhysicsEngine* physicsEngine, Bullet*heroBullet, glm::vec3 position, GLfloat height,int type)
:Position(position),TargetPos(position),velocity(0.0f),acceler(0.0f)
{
    firstupdate = 1;
    explode_first = 1;
    explodeStartTime=0.0f;
    IsDead = 0;
    explode = 0;
    age = 0.0f;
    this->type=type;
    this->height=height;
    this->radius=height*HRrate;
    
    this->heroBullet=heroBullet;
    this->physicsEngine = physicsEngine;
    VertVelocity = glm::vec3(0.0f);
    accelerUp = glm::vec3(0.0f);
    isJumping = false;

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

void Crystal::jump()
{
    if(!isJumping){
        VertVelocity = glm::vec3(0.0f, JumpInitialSpeed, 0.0f);
        accelerUp.y = 0.0f;
        isJumping = true;
    }
}


void Crystal::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,vertexs.size()/8);
    glBindVertexArray(0);
}

void Crystal::updateState()
{
    glm::vec3 sp = glm::vec3(Position.x,Position.y + height/2.0f,Position.z) - heroBullet->startPos;
    float spdotd=glm::dot(sp,heroBullet->direction);
    float lensp=glm::length(sp);
    float dist2=lensp*lensp-spdotd*spdotd;
    if(heroBullet->IsAttack() || heroBullet->Hitted){
        if(dist2 < RaiusRate * radius)explode = 1;
    }
}

void Crystal::updatePosition(const glm::vec3 cameraPos, const GLfloat deltaTime)
{
    glm::vec3 direc=cameraPos - Position;
    float distance = glm::length(glm::vec3(direc.x,0.0f,direc.z));

    if(firstupdate)velocity = glm::vec3(0.0f,0.0f,-1.0f) * CRYSTAL_SPEED * deltaTime;
    else if(distance < RaiusRate * radius){
        if(!type)velocity = glm::normalize( glm::vec3(direc.x,0.0f,direc.z) ) * -1.8f * CRYSTAL_SPEED * deltaTime;
        else velocity = glm::normalize( glm::vec3(direc.x,0.0f,direc.z) ) * CRYSTAL_SPEED * deltaTime;
    }
    else if(distance < CloseRate * CloseEnough){
        if(!type)velocity = glm::normalize( glm::vec3(direc.x,0.0f,direc.z) ) * 1.8f * CRYSTAL_SPEED * deltaTime;
        else velocity = glm::normalize( glm::vec3(direc.x,0.0f,direc.z) ) * CRYSTAL_SPEED * deltaTime;
    }
    else if(distance < CloseEnough){
        velocity = glm::normalize( glm::vec3(direc.x,0.0f,direc.z) ) * CRYSTAL_SPEED * deltaTime;
    }
    else{
        acceler = glm::normalize(glm::vec3(velocity.z,0.0f,-velocity.x)) * AccelerFactor * CRYSTAL_SPEED * deltaTime;
        velocity += acceler;
        velocity=glm::normalize(velocity) * CRYSTAL_SPEED * deltaTime;
    }
    
    Position += velocity;
    TargetPos = Position + 10.0f * velocity;

    // eyePos for collision
    glm::vec3 eyePos = glm::vec3(Position.x, Position.y + height, Position.z);
    glm::vec3 eyeTarg= glm::vec3(TargetPos.x, TargetPos.y + height, TargetPos.z);
    physicsEngine->outCollisionTest(eyePos, eyeTarg);
    physicsEngine->inCollisionTest(eyePos, eyeTarg, height);
    physicsEngine->updateCameraVertMovement(eyePos,eyeTarg,VertVelocity,accelerUp,isJumping,height,deltaTime);
	
    Position = glm::vec3(eyePos.x, eyePos.y - height, eyePos.z);
    TargetPos= glm::vec3(eyeTarg.x, eyeTarg.y - height, eyeTarg.z);
    if(Position.y < 0.0f)
	{
		Position.y = 0.0f;
		isJumping=false;
	}
    if(firstupdate)firstupdate=0;
}

//###################### CrystalSystem ######################

void CrystalSystem::generateCrystal(glm::vec3 centerPos,float areaRadius,float frequency,float goodRate,float curTime)
{
    if( curTime - lastTime > 1.0f / frequency){
        lastTime = curTime;

        float theta = (float)(rand()%180)/180.0f * PI;
        float r=(float)(rand()%15)/15.0f * areaRadius;
        int type = 0;
        if(rand()%100 < goodRate*100)type = 1;
        glm::vec3 position;
        position.x = centerPos.x + r * sin(theta);
        position.y = centerPos.y;
        position.z = centerPos.z + r * cos(theta);
        addCrystal(position, 2.5f, type);

        std::map<int,Crystal>::iterator p;
        for(p=GoodCrystals.begin();p!=GoodCrystals.end();p++){
            p->second.ageIncrease( 1.0f / frequency );
            if(p->second.getAge() > LifeTime)p->second.die();
        }
        for(p=BadCrystals.begin();p!=BadCrystals.end();p++){
            p->second.ageIncrease( 1.0f / frequency );
            if(p->second.getAge() > LifeTime)p->second.die();
        }
    }
}


void CrystalSystem::addCrystal(glm::vec3 position, GLfloat height, int type)
{
    if(type)GoodCrystals.insert(std::make_pair(goodCnt++,Crystal(physicsEngine,heroBullet,position,height,1)));
    else BadCrystals.insert(std::make_pair(badCnt++,Crystal(physicsEngine,heroBullet,position,height,0)));
}

void CrystalSystem::updateAll(const glm::vec3 cameraPos, const GLfloat deltaTime)
{
    std::map<int,Crystal>::iterator p,o;
    for(p=GoodCrystals.begin();p!=GoodCrystals.end();p++){
        if(rand()%200<5)p->second.jump();
        p->second.updatePosition(cameraPos,deltaTime);
        p->second.updateState();

        // crystals 之间的碰撞
        for(o=GoodCrystals.begin();o!=GoodCrystals.end();o++)if(o!=p){
            glm::vec3 direc = p->second.Position - o->second.Position;
            float dist = glm::length(direc);
            if(dist < 1.0f*(p->second.getRadius()+o->second.getRadius())){
                p->second.Position += glm::normalize(direc) * CRYSTAL_SPEED * deltaTime;
            }
        }
        for(o=BadCrystals.begin();o!=BadCrystals.end();o++)if(o!=p){
            glm::vec3 direc = p->second.Position - o->second.Position;
            float dist = glm::length(direc);
            if(dist < 1.0f*(p->second.getRadius()+o->second.getRadius())){
                p->second.Position += glm::normalize(direc) * CRYSTAL_SPEED * deltaTime;
            }
        }
    }
    for(p=BadCrystals.begin();p!=BadCrystals.end();p++){
        if(rand()%200<5)p->second.jump();
        p->second.updatePosition(cameraPos,deltaTime);
        p->second.updateState();

        // crystals 之间的碰撞
        for(o=GoodCrystals.begin();o!=GoodCrystals.end();o++)if(o!=p){
            glm::vec3 direc = p->second.Position - o->second.Position;
            float dist = glm::length(direc);
            if(dist < 1.0f*(p->second.getRadius()+o->second.getRadius())){
                p->second.Position += glm::normalize(direc) * CRYSTAL_SPEED * deltaTime;
            }
        }
        for(o=BadCrystals.begin();o!=BadCrystals.end();o++)if(o!=p){
            glm::vec3 direc = p->second.Position - o->second.Position;
            float dist = glm::length(direc);
            if(dist < 1.0f*(p->second.getRadius()+o->second.getRadius())){
                p->second.Position += glm::normalize(direc) * CRYSTAL_SPEED * deltaTime;
            }
        }
    }
}

void CrystalSystem::drawAll(const glm::mat4 &projection,const glm::mat4 &view,const glm::vec3 &cameraPos,const unsigned int skyboxID,float curTime,float deltaTime,int &score)
{
    std::map<int,Crystal>::iterator p;
    CryShader.use();
        CryShader.setMat4("projection",projection);
        CryShader.setMat4("view",view);
        CryShader.setVec3("cameraPos",cameraPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
        CryShader.setInt("skybox",0);
        CryShader.setFloat("time", curTime);
        CryShader.setVec3("color",glm::vec3(0.0f,0.0f,0.5f));//good color blue
        for(p=GoodCrystals.begin();p!=GoodCrystals.end();p++)if(p->second.IsOk()){
            if(p->second.IsExploding() && p->second.explode_first)
			{
				p->second.explode_first = false;
                p->second.explodeStartTime = curTime;// explode start time
				CryShader.setFloat("initTime", curTime);
			}else if (!p->second.IsExploding()){
				p->second.explode_first = true;
			}
            if(curTime - p->second.explodeStartTime > EXPLODE_TIME){
                p->second.explode=0;
                if(!p->second.explode_first){
                    p->second.die();
                    score--;//shoot good one, score decrease!
                }
            }
            CryShader.setBool("explode_now", p->second.IsExploding());
            glm::mat4 model=glm::mat4(1.0f);
            model=glm::translate(model,p->second.Position);
            CryShader.setMat4("model", model);
            p->second.draw();//draw
        }
        CryShader.setVec3("color",glm::vec3(0.8f,0.0f,0.0f));//good color blue
        for(p=BadCrystals.begin();p!=BadCrystals.end();p++)if(p->second.IsOk()){
            if(p->second.IsExploding() && p->second.explode_first)
			{
				p->second.explode_first = false;
                p->second.explodeStartTime = curTime;// explode start time
				CryShader.setFloat("initTime", curTime);
			}else if (!p->second.IsExploding()){
				p->second.explode_first = true;
			}
            if(curTime - p->second.explodeStartTime > EXPLODE_TIME){
                p->second.explode=0;
                if(!p->second.explode_first){
                    p->second.die();
                    score++;//shoot bad one, score increase!
                }
            }
            CryShader.setBool("explode_now", p->second.IsExploding());
            glm::mat4 model=glm::mat4(1.0f);
            model=glm::translate(model,p->second.Position);
            CryShader.setMat4("model", model);
            p->second.draw();//draw
        }
    glUseProgram(0);
    
    deleteAllDead();
}

void CrystalSystem::updateHeroState(const glm::vec3 &cameraPos,int &closeEnough,int &damage,int &bullet)
{
    closeEnough = 0;
    std::map<int,Crystal>::iterator p;
    for(p=BadCrystals.begin();p!=BadCrystals.end();p++)if(p->second.IsOk()){
        glm::vec3 direc = glm::vec3(cameraPos.x,cameraPos.y - HeroHeight,cameraPos.z) - p->second.Position;
        if(glm::length(direc) < BloodViewRate * CloseEnough)
            closeEnough = 1;// enable blood view
        if(glm::length(direc) < RaiusRate * p->second.getRadius())
            damage++;// be attacked and loss blood
    }
    for(p=GoodCrystals.begin();p!=GoodCrystals.end();p++)if(p->second.IsOk()){
        glm::vec3 direc=glm::vec3(cameraPos.x,cameraPos.y - HeroHeight,cameraPos.z) - p->second.Position;
        if(glm::length(direc) < RaiusRate * p->second.getRadius()){
            bullet++;// get bullets
            p->second.die();
        }
    }
}


void CrystalSystem::deleteAllDead()
{
    std::map<int,Crystal>::iterator p,tmp;
    for(p=BadCrystals.begin();p!=BadCrystals.end();){
        if(!p->second.IsOk()){
            tmp = p++;
            BadCrystals.erase(tmp);
        }
        else p++;
    }
    
    for(p=GoodCrystals.begin();p!=GoodCrystals.end();){
        if(!p->second.IsOk()){
            tmp = p++;
            GoodCrystals.erase(tmp);
        }
        else p++;
    }
}
