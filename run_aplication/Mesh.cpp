#include "Mesh.h"

//Mesh's Function
Mesh::Mesh(std::vector<Vertex> outVertices, std::vector<unsigned int> outIndeces, std::vector<Texture> outTexture) {
	vertices = outVertices;
	indeces = outIndeces;
	texture = outTexture;

	mesh_setup();
}

//Draw Mesh
//draw without instance
void Mesh::Draw(Shader& shader) {
	unsigned int diffuse = 1;
	unsigned int specular = 1;
	for (unsigned int i = 0; i < texture.size(); i++, diffuse++, specular++) {
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = texture[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuse);
		else if (name == "texture_specular")
			number = std::to_string(specular);

		shader.setInt("material." + name + number, i);
		glBindTexture(GL_TEXTURE_2D, texture[i].id);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indeces.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}
//draw with instance
void Mesh::Draw(Shader& shader, int primcount) {
	unsigned int diffuse = 1;
	unsigned int specular = 1;
	for (unsigned int i = 0; i < texture.size(); i++, diffuse++, specular++) {
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = texture[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuse);
		else if (name == "texture_specular")
			number = std::to_string(specular);

		shader.setInt("material." + name + number, i);
		glBindTexture(GL_TEXTURE_2D, texture[i].id);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, indeces.size(), 
							GL_UNSIGNED_INT, 0, primcount);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

//setup mesh(VAO, VBO, EBO) without instance
void Mesh::mesh_setup() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indeces.size() * sizeof(unsigned int), &indeces[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	//bone ids
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_boneIDs));
	//weight
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_weights));

	glBindVertexArray(0);
}
//setup mesh(VAO, VBO, EBO) with instance
void Mesh::mesh_setupInstance(int size, glm::mat4* data) {
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	// vertex attributes
	std::size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}