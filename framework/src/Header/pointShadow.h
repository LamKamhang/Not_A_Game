#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "config.h"
#include "Shader.h"


class PointShadow
{
private:
    GLuint SHADOW_WIDTH;
    GLuint SHADOW_HEIGHT;
    GLfloat near_plane;
    GLfloat far_plane;
    glm::vec3 lightPos;
    glm::mat4 shadowProj;
    std::vector<glm::mat4> shadowTransforms;
    GLuint FBO;
    GLuint depthCubemap;
public:
    PointShadow(const glm::vec3 &lightPos);
    ~PointShadow();
    GLuint GetDepthCubemap(){return depthCubemap;}
    void pre_use(Shader &shader);
    void draw(Shader &shader);
};