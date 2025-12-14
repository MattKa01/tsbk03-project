#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"


float quadVertices[] = {
    // pos        // uv (-1..1)
    -1.0f, -1.0f,  -1.0f, -1.0f,
     1.0f, -1.0f,   1.0f, -1.0f,
     1.0f,  1.0f,   1.0f,  1.0f,

    -1.0f, -1.0f,  -1.0f, -1.0f,
     1.0f,  1.0f,   1.0f,  1.0f,
    -1.0f,  1.0f,  -1.0f,  1.0f
};

float vertices[] = {
    -1.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  0.0f,
     1.0f, -1.0f,  0.0f,
    -1.0f, -1.0f,  0.0f
};
GLuint indices[] = {
    0, 1, 2,
    2, 3, 0
};

GLuint VBO, VAO;
GLFWwindow* window;


// Callback function for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0,width,height);
}

// Handle any form of input while rendering
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window and set it as current context
    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    // Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // Tell opengl the window rendering size
    glViewport(0,0,1000,800);

    // Set window resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

   }

void display(Shader shaders) {

    int uTime = 1;
    // Rendering loop
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Handle input
        processInput(window);

        // Rendering commands here
        shaders.use();
        int wHeight, wWidth;
        glfwGetWindowSize(window, &wWidth, &wHeight);
        glm::vec2 resolution = glm::vec2(wWidth, wHeight);

        shaders.setInt("time", uTime);
        glUniform2fv(glGetUniformLocation(shaders.ID, "resolution"),1, glm::value_ptr(resolution));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        uTime++;
    }
}

int main() {
    // Initialize context
    init();
    // Set up shaders
    Shader shaders = Shader("main.vert", "main.frag");
    // Display rendering
    display(shaders);
    // Terminate
    glfwTerminate();
    return 0;
}
