#pragma once

#include "../DeanLib/include/DeanLibDefines.h"
#include "../DeanLib/include/CircularQueue.h"
#include "../DeanLib/include/Trackable.h"

#include "MemoryPool.h"
#include "Particle.h"
#include <vector>
#include "../MeshSystem/MeshRenderer.h"

#include "../MeshSystem/meshGenerator.h"

class ParticleSystem : public Trackable
{
public:
	ParticleSystem(int poolSize, Shader* newShader, Mesh mMesh);
	~ParticleSystem();

	void updateSystem(float currentTime, float deltaTime);
	void renderSystem();
	void addParticle(float sTime);
	void removeParticle(Particle* pParticleToDestroy);

private:
	MemoryPool particlePool;
	std::vector<Particle*> particleVec;

	meshSystem::Mesh particleMesh;
	macroLib::Shader* particleShader;

	float lastParticleSpawn = 0.0f;

	const float timeBetweenSpawns = 0.6f;


};
