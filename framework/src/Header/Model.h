#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "util.h"

// model class
class Model 
{
public:
	// constructor and destructor
	Model(const std::string &path, GLboolean gamma = false);
	~Model();

	// draw the model
	GLvoid Draw(Shader shader);

	// get model data
	const std::vector<Texture>& GetLoadedTextures();
	const std::vector<Mesh>& GetMeshes();
	const std::string& GetDirectory();
	GLboolean GetGammaCorrection();

	// set model data
	GLvoid SetMeshes(const std::vector<Mesh> &meshes);
	GLvoid SetGammaCorrection(const GLboolean &gamma);
	GLvoid SetDirectory(const std::string &directory);

private:
	std::vector<Texture> LoadedTextures;
	std::vector<Mesh> Meshes;
	std::string Directory;
	GLboolean GammaCorrection;

	// loads a model with ASSIMP extensions from file
	// stores the resulting meshes in the meshes vector
	GLvoid loadModel(const std::string &path);

	// processes a node in recursive fashion
	// Processes each individual mesh located at the node 
	// and repeats this process on its children nodes
	GLvoid processNode(const aiNode *node, const aiScene *scene);

	const Mesh processMesh(const aiMesh *mesh, const aiScene *scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet. [optimization]
    // the required info is returned as a Texture struct.
	const std::vector<Texture> loadMaterialTextures(const aiMaterial *mat, const aiTextureType type, const std::string &typeName);
};