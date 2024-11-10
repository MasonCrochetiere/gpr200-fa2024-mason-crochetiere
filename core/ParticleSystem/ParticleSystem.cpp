#include "ParticleSystem.h"
using namespace std;

ParticleSystem::ParticleSystem(int poolSize, Shader* mShader, Mesh mMesh)
	:particlePool(poolSize, sizeof(Particle)), particleShader(mShader), particleMesh(mMesh)
{
	particleShader = mShader;
	particleMesh = mMesh;
	//meshSystem::MeshData cubeMeshData;
	////meshSystem::generatePlane(5.0f, 5.0f, 32, &cubeMeshData);
	//meshSystem::generateCube(1.0f, &cubeMeshData);
	//particleMesh = meshSystem::Mesh(cubeMeshData);
}

ParticleSystem::~ParticleSystem()
{
	for (unsigned int i = 0; i < particleVec.size(); i++)
	{
		Particle* target = (Particle*)particleVec.at(i);

		target->~Particle();
		particlePool.freeObject((Byte*)target);

	}
	particleVec.clear();
}

void ParticleSystem::updateSystem(float currentTime, float deltaTime)
{
	if (currentTime - lastParticleSpawn >= timeBetweenSpawns)
	{
		cout << "Making particle!" << endl;
		// make particle
		addParticle(currentTime);
		lastParticleSpawn = currentTime;
	}

	for (unsigned int i = 0; i < particleVec.size(); i++)
	{
		if (currentTime - particleVec.at(i)->getStartTime() >= particleLifetime)
		{
			particlesToDestroyVec.push_back(particleVec.at(i));
		}
	}

	destroyQueuedParticles();
}

void ParticleSystem::destroyQueuedParticles()
{
	for (unsigned int i = 0; i < particlesToDestroyVec.size(); i++)
	{
		removeParticle(particlesToDestroyVec.at(i));
	}
	particlesToDestroyVec.clear();
}

void ParticleSystem::renderSystem()
{
	for (unsigned int i = 0; i < particleVec.size(); i++)
	{
		Particle* pTarget = particleVec.at(i);
		pTarget->renderParticle();
	}
}

void ParticleSystem::addParticle(float sTime)
{
	Byte* ptr = particlePool.allocateObject();
	if (ptr != NULL)
	{
		//std::cout << "MAKING A UNIT" << endl;
		Particle* pParticle = new (ptr)Particle(particleMesh, *particleShader, sTime);

		particleVec.push_back(pParticle);
		//pFireBall->mPooledObject = true;
	}
}

void ParticleSystem::removeParticle(Particle* pParticleToDestroy)
{
	for (unsigned int i = 0; i < particleVec.size(); i++)
	{
		Particle* pTarget = particleVec.at(i);
		if (pTarget == pParticleToDestroy)
		{
			pTarget->~Particle();
			particlePool.freeObject((Byte*)pTarget);

			particleVec.erase(particleVec.begin() + i);

			break;
		}

	}
}

