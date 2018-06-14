#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include <vector>
#include <string>
#include <iostream>

GLuint loadTexture(const std::string &fileName, const std::string &directory = std::string("."), GLboolean gamma = false);
GLuint loadCubemap(const std::vector<std::string> &faces, const std::string &directory = std::string("."));
