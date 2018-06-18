#include <glad/glad.h>
#include <vector>
#include "Shader.h"
#define MaxAlpha 0.5f
#define MinAlpha 0.25f
#define speed 2.0

class UI{
private:
    unsigned int uiVAO,uiVBO;
    std::vector<float>UIvertexs;
    float alpha;
    Shader uiShader;
public:
    UI():uiShader("Resource/Shader/ui.vs","Resource/Shader/ui.fs")
    {
        alpha=0.5;
        float tmp[]={
            -1.0f,-1.0f,0.0f,
            1.0f,-1.0f,0.0f,
            1.0f,1.0f,0.0f,
            
            1.0f,1.0f,0.0f,
            -1.0f,1.0f,0.0f,
            -1.0f,-1.0f,0.0f
        };
        for(int i=0;i < 18 ;i++)UIvertexs.push_back(tmp[i]);
        glGenVertexArrays(1,&uiVAO);
        glBindVertexArray(uiVAO);
            glGenBuffers(1,&uiVBO);
            glBindBuffer(GL_ARRAY_BUFFER,uiVBO);
                glBufferData(GL_ARRAY_BUFFER,UIvertexs.size() * sizeof(float),&UIvertexs[0],GL_STATIC_DRAW);
                glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(GLvoid*)(0));
                glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }
    void updateAlpha(int State, float curTime){
        if(State==0)alpha=0.0f;
        else if(State==1){
            alpha = (MaxAlpha-MinAlpha) * sin(curTime * speed) + MinAlpha;
        }
    }
    void draw(){
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(uiVAO);
        uiShader.use();
        uiShader.setFloat("alpha",alpha);
            glDrawArrays(GL_TRIANGLES,0,UIvertexs.size()/3);
		glUseProgram(0);
		glBindVertexArray(0);
		glDisable(GL_BLEND);
    }
};
