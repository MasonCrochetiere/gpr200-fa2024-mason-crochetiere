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
	if (currentTime - lastParticleSpawn >= particleValues.timeBetweenSpawns)
	{
		//cout << "Making particle!" << endl;
		// make particle
		addParticle(currentTime);
		lastParticleSpawn = currentTime;
	}

	for (unsigned int i = 0; i < particleVec.size(); i++)
	{
		if (currentTime - particleVec.at(i)->getStartTime() >= particleValues.particleLifetime)
		{
			particlesToDestroyVec.push_back(particleVec.at(i));
		}
	}

	destroyQueuedParticles();

	for (unsigned int i = 0; i < particleVec.size(); i++)
	{
		particleVec.at(i)->pRenderer->transform.position += (particleVec.at(i)->velocityOffset * particleValues.getVelocity(deltaTime, currentTime, particleVec.at(i)->getStartTime()));
	}
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

void ParticleSystem::setSystemValues(ParticleSystemValues values)
{
	particleValues = values;
}

void ParticleSystem::addParticle(float sTime)
{
	Byte* ptr = particlePool.allocateObject();
	if (ptr != NULL)
	{
		//std::cout << "MAKING A UNIT" << endl;
		Particle* pParticle = new (ptr)Particle(particleMesh, *particleShader, sTime);

		if (particleValues.useRandomPosition)
		{
			glm::vec3 pos(1.0);

			pos.x = RandomRange(particleValues.randomPosMin.x, particleValues.randomPosMax.x);
			pos.y = RandomRange(particleValues.randomPosMin.y, particleValues.randomPosMax.y);
			pos.z = RandomRange(particleValues.randomPosMin.z, particleValues.randomPosMax.z);

			pParticle->pRenderer->transform.position = pos;
		}
		else
		{
			pParticle->pRenderer->transform.position = particleValues.position;
		}

		pParticle->pRenderer->transform.rotation = particleValues.rotation;
		pParticle->pRenderer->transform.scale = particleValues.scale;

		glm::vec3 vOffset(1.0);

		if (particleValues.useRandomVelocity)
		{
			vOffset.x = RandomRange(particleValues.randomVelocityMin.x, particleValues.randomVelocityMax.x);
			vOffset.y = RandomRange(particleValues.randomVelocityMin.y, particleValues.randomVelocityMax.y);
			vOffset.z = RandomRange(particleValues.randomVelocityMin.z, particleValues.randomVelocityMax.z);
		}

		pParticle->velocityOffset = vOffset;

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

