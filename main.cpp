#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"


#define near 1.0
#define far 200.0
#define right 0.6
#define left -0.6
#define top 0.6
#define bottom -0.6

GLfloat projectionMatrix[] = {
    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
    0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
    0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
    0.0f, 0.0f, -1.0f, 0.0f };


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

GLuint VBO, VAO, EBO;
GLFWwindow* window;
glm::mat4 look = glm::lookAt(
    glm::vec3(0.0f, 0.0f, -8.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));

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
    glViewport(0,0,800,600);

    // Set window resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Set up VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Set up VBO
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
   }

void display(Shader shaders) {

    glUniformMatrix4fv(glGetUniformLocation(shaders.ID,"lookAt"), 1, GL_FALSE, glm::value_ptr(look));
    glUniformMatrix4fv(glGetUniformLocation(shaders.ID,"projection"), 1, GL_FALSE, &projectionMatrix[0]);
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
        //glm::vec2 resolution = glm::vec2(wWidth, wHeight);
        //glUniform2iv(glGetUniformLocation(shaders.ID, "Resolution"),2, );
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);
        // Check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
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
