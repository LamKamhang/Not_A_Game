#include "Header/config.h"

namespace settings{
    // setting
    const GLuint SCR_WIDTH = 1200;
    const GLuint SCR_HEIGHT = 900;
    extern const GLfloat PROJECT_NEAR = 0.1f;
    extern const GLfloat PROJECT_FAR = 500.0f;

    enum button_mode cur_button_mode;

    // camera
    Camera camera(glm::vec3(0.0f, 2.5f, 5.0f));

    // timing
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    bool explode = false;
    bool phong = false;
    bool flashlight_on = false;

    // API
    GLuint init_setting(GLFWwindow *&window)
    {
        // glfw: initialize and configure
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        // uncomment this statement to fix compilation on OS X
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif // __APPLE__

        // glfw window creation
        
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Not A Game", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return 0;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        
        // capture the mouse
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // glad: load all OpenGL fucniton pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return 0;
        }

        // configure global opengl state
        glEnable(GL_DEPTH_TEST);
        return 1;
    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    GLvoid processInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)//w
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)//a
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)//s
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)//d
            camera.ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)//hero can jump!
            camera.ProcessKeyboard(JUMP, deltaTime);
        
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
            explode = true;
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            explode = false;
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            phong = true;
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
            phong = false;
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            flashlight_on = true;
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
            flashlight_on = false;

        if(cur_button_mode == left)
            ;// camera.ProcessMouseButton(MB_LEFT,deltaTime);

        camera.LoopFunction(deltaTime);
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    GLvoid framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    // glfw: whenever the mouse moves, this callback is called
    GLvoid mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
    {
        static GLdouble lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;
        static bool firstMove = true;
        GLdouble xoffset,yoffset;

        if (firstMove)
        {
            lastX = xpos;
            lastY = ypos;
            firstMove = false;
        }
        xoffset = xpos - lastX;
        yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;
        camera.ProcessMouseMovement(xoffset, yoffset);
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    GLvoid scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset)
    {
        camera.ProcessMouseScroll(yoffset);
    }

    GLvoid mouse_button_callback(GLFWwindow *window, GLint Button, GLint Action, GLint Mode)
    {
        if (Action == GLFW_PRESS)
        {
            switch (Button)
            {
            case GLFW_MOUSE_BUTTON_LEFT	 : cur_button_mode = left	; break;
            case GLFW_MOUSE_BUTTON_MIDDLE: cur_button_mode = middle	; break;
            case GLFW_MOUSE_BUTTON_RIGHT : cur_button_mode = right	; break;
            default:					   cur_button_mode = none	; break;
            }
        }
        else
        {
            cur_button_mode = none;
        }
    }
}