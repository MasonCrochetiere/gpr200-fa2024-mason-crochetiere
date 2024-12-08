#pragma once

#include "../DeanLib/include/DeanLibDefines.h"
#include "../DeanLib/include/CircularQueue.h"
#include "../DeanLib/include/Trackable.h"

#include "MemoryPool.h"
#include "Particle.h"
#include <vector>
#include "../MeshSystem/MeshRenderer.h"

#include "../MeshSystem/meshGenerator.h"

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

private:
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

};