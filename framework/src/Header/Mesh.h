#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

// define a vertex structure to describe a mesh vertex
struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoord
	glm::vec2 TexCoord;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

// define a texture to describe a mesh texture
struct Texture {
	// texture's id, use it to distinguish different texture
	GLuint id;
	// texture type, such as specular, diffuse and so on.
	std::string type;
	// the path to load the texture.
	std::string path;
};

// the Mesh class
class Mesh {
public:
	// contructor and destructor
	Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Texture> &textures);
	~Mesh();

	// render the mesh
	GLvoid Draw(Shader shader);

	// get the mesh's data
	const std::vector<Vertex>& GetVertices();
	const std::vector<GLuint>& GetIndices();
	const std::vector<Texture>& GetTextures();
	GLuint GetVAO();

	// set the mesh's data
	GLvoid SetVertices(const std::vector<Vertex> &vertices);
	GLvoid SetIndices(const std::vector<GLuint> &indices);
	GLvoid SetTextures(const std::vector<Texture> &textures);
private:
	// render data
	std::vector<Vertex> Vertices;
	std::vector<GLuint> Indices;
	std::vector<Texture> Textures;
	GLuint VAO, VBO, EBO;

	GLvoid setupMesh();
};