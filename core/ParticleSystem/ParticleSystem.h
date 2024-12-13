// This particle system class was created by Mason Crochetiere
// it pulls from Dean Lawson's library to make it function within the Object Pool it is used in

#pragma once

#include "../DeanLib/include/DeanLibDefines.h"
#include "../DeanLib/include/CircularQueue.h"
#include "../DeanLib/include/Trackable.h"

#include "MemoryPool.h"
#include "Particle.h"
#include <vector>
#include "../MeshSystem/MeshRenderer.h"

#include "../MeshSystem/meshGenerator.h"
#include "../Lighting/LightingSystem.h"

#include "ParticleSystemValues.h"

class ParticleSystem : public Trackable
{
public:
	ParticleSystem(int poolSize, Shader* newShader, meshSystem::Mesh mMesh);
	~ParticleSystem();

	void updateSystem(float currentTime, float deltaTime, glm::vec3 cameraPos);
	void destroyQueuedParticles();
	void renderSystem();
	void setSystemValues(ParticleSystemValues values);

	void addParticle(float sTime);
	void removeParticle(Particle* pParticleToDestroy);

	void setLightingSystemRef(lightSystem::LightingSystem* system);

private:
	lightSystem::LightingSystem* lightSystemRef;

	MemoryPool particlePool;
	std::vector<Particle*> particleVec;

	// when you want to destroy a particle, add it to this vector
	// this way we can delete them all elegantly each frame
	// and not cause any collection enumeration issues
	std::vector<Particle*> particlesToDestroyVec;

	meshSystem::Mesh particleMesh;
	macroLib::Shader* particleShader;

	ParticleSystemValues particleValues;

	float lastParticleSpawn = 0.0f;

	float avoidRadius = 6.0f;
	float avoidSpeed = 10.0f;

};