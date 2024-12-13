// This particle struct was created by Mason Crochetiere
// It is simply a list of values utilized by the particle system to manipulate particles
// The base values are set to perform the firefly particle simulation

#pragma once
#include <glm/glm.hpp>


struct ParticleSystemValues
{
	float timeBetweenSpawns = 0.1f;
	float particleLifetime = 2.0f;

	glm::vec3 position = glm::vec3(0.0, 0.0, -5.0);
	glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 scale = glm::vec3(0.1, 0.1, 0.1);

	glm::vec3 particleVelocity = glm::vec3(4.0, 4.0, 4.0);

	bool useRandomVelocity = true;
	glm::vec3 randomVelocityMin = glm::vec3(-1.0, -1.0, -1.0);
	glm::vec3 randomVelocityMax = glm::vec3(1.0, 1.0, 1.0);

	bool useRandomPosition = true;
	glm::vec3 randomPosMin = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 randomPosMax = glm::vec3(10.0, 3.0, 10.0);

	bool sinVelocity = true;
	bool cosVelocity = true;
	bool tanVelocity = false;

	glm::vec3 getVelocity(float deltaTime, float currentTime, float startTime);
};
