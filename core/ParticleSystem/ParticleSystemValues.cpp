#include "ParticleSystemValues.h"

glm::vec3 ParticleSystemValues::getVelocity(float deltaTime, float currentTime, float startTime)
{
	glm::vec3 vec = particleVelocity;

	vec.x *= cosVelocity ? glm::cos(currentTime - startTime) : 1;
	vec.y *= sinVelocity ? glm::sin(currentTime - startTime) : 1;

	vec *= deltaTime;
	return vec;
}