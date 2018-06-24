#include "Header/Render.h"
using namespace settings;

Framebuffer::Framebuffer():
    shader("Resource/Shader/render.vs", "Resource/Shader/render.fs"),
    quadVertices({	
    // positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f})
{
    // screen quad VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, quadVertices.size()*sizeof(float), &quadVertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    shader.use();
    shader.setInt("screenTexture", 0);
    // framebuffer configuration
    unsigned int framebuffer;
    glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        // create a color attachment texture
        glGenTextures(1, &TBO);
            glBindTexture(GL_TEXTURE_2D, TBO);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TBO, 0);
        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        glGenRenderbuffers(1, &RBO);
            glBindRenderbuffer(GL_RENDERBUFFER, RBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); 
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); 
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


Framebuffer::~Framebuffer()
{
    glDeleteBuffers(1, &TBO);
    glDeleteBuffers(1, &RBO);
    glDeleteBuffers(1, &FBO);
}

void Framebuffer::pre_use()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    shader.setBool("Inversion", Inversion);
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, TBO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}