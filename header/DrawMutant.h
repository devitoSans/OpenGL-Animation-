#pragma once

#include "Shader.h"
#include "Shader.h"
#include "Model.h"
#include "Animator.h"
#include "Game_Engine.h"


void drawMutant(Shader& shader, Animator& animator,
	GameEngine* game, Model& model, glm::vec3 mutantPos, glm::mat4 rotate) {
	shader.useProgram();

	auto transform = animator.getFinalMatrices();
	for (int i = 0; i < transform.size(); i++)
		shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transform[i]);

	game->shininess = 16.f;
	game->attenuation = glm::vec3(1.0, 0.14, 0.07);
	game->lightPos = glm::vec3(2.f, 5.f, 2.f);
	game->ambientLight = glm::vec3(0.05f);
	
	game->op_model = glm::translate(game->op_model, mutantPos);
	shader.setMat4("modelCPP", rotate);
	game->drawSetup(&shader);
	model.Draw(shader);
}