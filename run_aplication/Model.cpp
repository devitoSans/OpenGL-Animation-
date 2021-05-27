#include "Model.h"

//Texture id

//Texture id
unsigned int TextureFromFile(const char* path, const std::string& directory) {
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, NULL, SOIL_LOAD_RGBA);
	if (image) {
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

//Model's Function
//
//load the model
void Model::loadModel(std::string path) {
	Assimp::Importer import_;
	std::string pathGeneral = "model/" + path;
	const aiScene* scene = import_.ReadFile(pathGeneral, 
					aiProcess_SplitLargeMeshes | 
					aiProcess_Triangulate | 
					aiProcess_FlipUVs |
					aiProcess_OptimizeMeshes |
					aiProcess_OptimizeGraph);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << import_.GetErrorString() << std::endl;
		std::cout << "ERROR::MODEL.CPP::FAILED_LOAD_MODEL\n";
		return;
	}

	directory = "model/" + path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

//draw the model that have loaded
//draw only specific component / mesh
void Model::Draw(Shader& shader, int atArray) {
	meshes[atArray].Draw(shader);
}
//draw all the components / meshes
void Model::Draw(Shader& shader) {
	for (auto& i : meshes)
		i.Draw(shader);
}
//draw all the components with instance
void Model::Draw(Shader& shader, unsigned int primcount) {
	for (auto& i : meshes)
		i.Draw(shader, (int)primcount);
}

//Setup Mesh with Instance
void Model::meshSetup_Instance(int size, glm::mat4* data) {
	for (auto& i : meshes)
		i.mesh_setupInstance(size, data);
}

//generate node and its children (if any)
void Model::processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

//read vertex, normal, texCoord, etc from the OBJ file
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indeces;
	std::vector<Texture> texture;

	//Vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector_3;
		glm::vec2 vector_2;

		//Bone Data
		setVertexBoneDataDefault(vertex);

		//position
		vector_3.x = mesh->mVertices[i].x;
		vector_3.y = mesh->mVertices[i].y;
		vector_3.z = mesh->mVertices[i].z;

		vertex.position = vector_3;

		//normal
		vector_3.x = mesh->mNormals[i].x;
		vector_3.y = mesh->mNormals[i].y;
		vector_3.z = mesh->mNormals[i].z;

		vertex.normal = vector_3;

		//texture coordinate
		if (mesh->mTextureCoords[0]) {
			vector_2.x = mesh->mTextureCoords[0][i].x;
			vector_2.y = mesh->mTextureCoords[0][i].y;

			vertex.texCoord = vector_2;
		}
		else
			vertex.texCoord = glm::vec2(0.f, 0.f);

		vertices.push_back(vertex);
	}

	//Indeces
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indeces.push_back(face.mIndices[j]);
	}

	//Material Texture
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//diffuse
		std::vector<Texture> diffuseMap = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		texture.insert(texture.end(), diffuseMap.begin(), diffuseMap.end());

		//specular
		std::vector<Texture> specularMap = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
		texture.insert(texture.end(), specularMap.begin(), specularMap.end());
	}

	//Extract bone's weight
	extractBoneWeight(vertices, mesh, scene);

	return Mesh(vertices, indeces, texture);
}

//set vertex's bone data to default
void Model::setVertexBoneDataDefault(Vertex& vertex) {
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
		vertex.m_boneIDs[i] = -1;
		vertex.m_weights[i] = 0.0f;
	}
}

//Extract bone's weight for vertices
void Model::extractBoneWeight(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene) {
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++) {
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		//check if this bone already exist or not
		if (m_boneInfoMap.find(boneName) == m_boneInfoMap.end()) {	//if not exist yet
			BoneInfo newBoneInfo;	//make a new bone
			newBoneInfo.id = m_boneCounter;
			newBoneInfo.offset = assimpHelper::ConvertMatrix(mesh->mBones[boneIndex]->mOffsetMatrix);
			m_boneInfoMap[boneName] = newBoneInfo;
			boneID = m_boneCounter;
			m_boneCounter++;
		}
		else {	//if already exist
			boneID = m_boneInfoMap[boneName].id;
		}

		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; weightIndex++) {
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			setVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

//set vertex's bone data
void Model::setVertexBoneData(Vertex& vertex, int boneID, float weight) {
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		if (vertex.m_boneIDs[i] < 0) {
			vertex.m_weights[i] = weight;
			vertex.m_boneIDs[i] = boneID;
			break;
		}
}

//load textures
std::vector<Texture> Model::loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string type_name) {
	std::vector<Texture> texture;
	std::vector<Texture> texture_loaded;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < texture_loaded.size(); j++) {
			if (std::strcmp(texture_loaded[j].path.data(), str.C_Str()) == 0) {
				texture.push_back(texture_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {   // if texture hasn't been loaded already, load it
			Texture tempTexture;
			tempTexture.id = TextureFromFile(str.C_Str(), directory);
			tempTexture.type = type_name;
			tempTexture.path = str.C_Str();

			texture.push_back(tempTexture);
			texture_loaded.push_back(tempTexture); // add to loaded textures
		}
	}

	return texture;
}