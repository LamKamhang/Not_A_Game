#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>


class Shader
{
public:
	// program ID
	GLuint ID;

	// constructor and destructor
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *tcsPath = nullptr, const GLchar *tesPath = nullptr, const GLchar *geometryPath = nullptr);
	~Shader();

	// use the program
	GLvoid use();

	// uniform tool function
	GLvoid setBool(const std::string &name, GLboolean value) const;
	GLvoid setInt(const std::string &name, GLint value) const;
	GLvoid setFloat(const std::string &name, GLfloat value) const;
	GLvoid setVec2(const std::string &name, const glm::vec2 &value) const;
	GLvoid setVec2(const std::string &name, GLfloat x, GLfloat y) const;
	GLvoid setVec3(const std::string &name, const glm::vec3 &value) const;
	GLvoid setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const;
	GLvoid setVec4(const std::string &name, const glm::vec4 &value) const;
	GLvoid setVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
	GLvoid setMat2(const std::string &name, const glm::mat2 &value) const;
	GLvoid setMat3(const std::string &name, const glm::mat3 &value) const;
	GLvoid setMat4(const std::string &name, const glm::mat4 &value) const;
	GLvoid setBlockBindPoint(const std::string &name, const GLuint value);

private:
	GLvoid checkCompileErrors(GLuint shader, const std::string &type);
};
