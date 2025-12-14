//
// Created by Mattias on 2025-11-18.
//

#ifndef TSBK03_PROJECT_SHADER_H
#define TSBK03_PROJECT_SHADER_H

#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"

/* Code taken from https://learnopengl.com/Getting-started/Shaders
 * Licensed under CC BY-NC 4.0 license as published by Creative Commons
 * By Joey De Vriez
 */

class Shader {
public:
    // Program ID
    unsigned int ID;

    // Constructor: Reads and builds shader
    Shader(const char* vertexPath, const char* fragmentPath);

    // Set shader as active
    void use();

    // Utility functions for uniforms
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 &value) const;
};


#endif //TSBK03_PROJECT_SHADER_H