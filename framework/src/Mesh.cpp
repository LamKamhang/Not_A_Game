#include "Header/Mesh.h"

// contructor and destructor
Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Texture> &textures):
	Vertices(vertices), Indices(indices), Textures(textures)
{
	setupMesh();
}
Mesh::~Mesh()
{
}

// render the mesh
GLvoid Mesh::Draw(Shader shader)
{
	// bind the appropriate textures
	GLuint diffuseNr = 0;
	GLuint specularNr = 0;
	GLuint normalNr = 0;
	GLuint heightNr = 0;

	for (GLuint i = 0; i < Textures.size(); ++i)
	{
		// active proper texture unit before binding
		glActiveTexture(GL_TEXTURE0 + i);
		// retrieve texture number (N --> diffuse_textureN)
		std::string number;
		std::string name = Textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		// set the uniform variable
		shader.setInt((name + number).c_str(), i);
		// finally bind the texture
		glBindTexture(GL_TEXTURE_2D, Textures[i].id);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// set everything back to defaults after configured
	glActiveTexture(GL_TEXTURE0);
}

// get the mesh's data
const std::vector<Vertex>& Mesh::GetVertices()
{
	return Vertices;
}
const std::vector<GLuint>& Mesh::GetIndices()
{
	return Indices;
}
const std::vector<Texture>& Mesh::GetTextures()
{
	return Textures;
}
GLuint Mesh::GetVAO()
{
	return VAO;
}

// set the mesh's data
GLvoid Mesh::SetVertices(const std::vector<Vertex> &vertices)
{
	Vertices = vertices;
	setupMesh();
}
GLvoid Mesh::SetIndices(const std::vector<GLuint> &indices)
{
	Indices = indices;
	setupMesh();
}
GLvoid Mesh::SetTextures(const std::vector<Texture> &textures)
{
	Textures = textures;
	setupMesh();
}

GLvoid Mesh::setupMesh()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind vertex array VAO to configure some propoties
	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	// load element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), &Indices[0], GL_STATIC_DRAW);

	/*
	预处理指令offsetof(s, m),
	它的第一个参数是一个结构体,
	第二个参数是这个结构体中变量的名字。
	这个宏会返回那个变量距结构体头部的字节偏移量(Byte Offset)
	*/
	// set up vertex attribute pointers
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoord));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Bitangent));

	// unbind it to keep individual
	glBindVertexArray(0);
}
