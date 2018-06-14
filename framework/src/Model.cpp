#include "Header/Model.h"


// model class

// constructor and destructor
Model::Model(const std::string &path, GLboolean gamma):
	GammaCorrection(gamma)
{
	loadModel(path);
}
Model::~Model()
{
}

// draw the model
GLvoid Model::Draw(Shader shader)
{
	for (GLuint i = 0; i < Meshes.size(); ++i)
		Meshes[i].Draw(shader);
}

// get model data
const std::vector<Texture>& Model::GetLoadedTextures()
{
	return LoadedTextures;
}
const std::vector<Mesh>& Model::GetMeshes()
{
	return Meshes;
}
const std::string& Model::GetDirectory()
{
	return Directory;
}
GLboolean Model::GetGammaCorrection()
{
	return GammaCorrection;
}

// set model data
GLvoid Model::SetMeshes(const std::vector<Mesh> &meshes)
{
	Meshes = meshes;
}
GLvoid Model::SetGammaCorrection(const GLboolean &gamma)
{
	GammaCorrection = gamma;
}
GLvoid Model::SetDirectory(const std::string &directory)
{
	Directory = directory;
}


// loads a model with ASSIMP extensions from file
// stores the resulting meshes in the meshes vector
GLvoid Model::loadModel(const std::string &path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// retrieve the directory path of the filepath
	Directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

// processes a node in recursive fashion
// Processes each individual mesh located at the node 
// and repeats this process on its children nodes
GLvoid Model::processNode(const aiNode *node, const aiScene *scene)
{
	// process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; ++i)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		Meshes.push_back(processMesh(mesh, scene));
	}
	// after processed all the meshes, do this recursively on the children nodes
	for (GLuint i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}

const Mesh Model::processMesh(const aiMesh *mesh, const aiScene *scene)
{
	// data to fill
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	// go through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; ++i)
	{	// we declare a placeholder vector 
		// since assimp uses its own vector class that doesn't directly convert to glm's vec3 class 
		// so we transfer the data to this placeholder glm::vec3 first.
		Vertex vertex;
		glm::vec3 vector; 
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // to check whether the model has textureCoords.
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoord = vec;
		}
		else
		{
			vertex.TexCoord = glm::vec2(0.0f, 0.0f);
		}
		// tangent
		if (mesh->mTangents)
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
		}
		// bitangent
		if (mesh->mBitangents)
		{
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 0 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps				   
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	for (auto &item : textures)
	{
		std::cout << item.type << ": " << item.path << std::endl;
	}
	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet. [optimization]
// the required info is returned as a Texture struct.
const std::vector<Texture> Model::loadMaterialTextures(const aiMaterial *mat, const aiTextureType type, const std::string &typeName)
{
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if the texture was loaded before.
		// if so, skip this one and continue to the next one
		bool skip = false;
		for (GLuint j = 0; j < LoadedTextures.size(); ++j)
		{
			if (std::strcmp(LoadedTextures[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(LoadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			std::string path(str.C_Str());
			std::size_t temp = _min(path.find_last_of('\\'), path.find_last_of('/'));
			std::size_t start = temp == std::string::npos ? 0 : temp;
			Texture texture;
			texture.id = loadTexture(path.substr(start).c_str(), Directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			LoadedTextures.push_back(texture);
		}
	}
	return textures;
}
