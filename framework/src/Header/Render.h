#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "config.h"
#include "Shader.h"


class Framebuffer
{
private:
    std::vector<float> quadVertices;
    GLuint quadVAO, quadVBO;
    GLuint FBO, TBO, RBO;
    Shader shader;
public:
    Framebuffer();
    ~Framebuffer();
    void pre_use();
    void draw();
};

////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////