#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

namespace settings{
    // settings
    extern bool starting;
    extern bool restart;
    extern const GLuint SCR_WIDTH;
    extern const GLuint SCR_HEIGHT;
    extern const GLfloat PROJECT_NEAR;
    extern const GLfloat PROJECT_FAR;
    extern bool explode;
    extern bool phong;

    enum button_mode {
        none, left, middle, right,
    };
    extern button_mode cur_button_mode;

    // camera
    extern Camera camera;

    // timing
    extern GLfloat deltaTime;
    extern GLfloat lastFrame;

    // callback API
    GLuint init_setting(GLFWwindow *&window);
    GLvoid framebuffer_size_callback(GLFWwindow* window, int width, int height);
    GLvoid mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
    GLvoid scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset);
    GLvoid processInput(GLFWwindow *window);
    GLvoid mouse_button_callback(GLFWwindow *window, GLint Button, GLint Action, GLint Mode);
}

