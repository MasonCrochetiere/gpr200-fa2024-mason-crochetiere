#pragma once
#include <glm/glm.hpp>


struct ParticleSystemValues
{
	float timeBetweenSpawns = 0.6f;
	float particleLifetime = 2.0f;

	glm::vec3 position = glm::vec3(0.0, 0.0, -5.0);
	glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);

	glm::vec3 particleVelocity = glm::vec3(1.0, 1.0, 1.0);

	bool sinVelocity = false;
	bool cosVelocity = false;
	bool tanVelocity = false;

	glm::vec3 getVelocity(float deltaTime, float currentTime, float startTime);
};
