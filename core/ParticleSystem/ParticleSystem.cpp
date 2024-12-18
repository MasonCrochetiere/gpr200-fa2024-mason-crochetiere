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

// This update particle system function handles particle emission and simulation;
// both of these belong in a 'game update' esc phase
// the rendering phase should happen alongside other rendering in main/etc and not alongside these simulative updates
void ParticleSystem::updateSystem(float currentTime, float deltaTime, glm::vec3 cameraPos)
{
	// Emission phase
	if (currentTime - lastParticleSpawn >= particleValues.timeBetweenSpawns)
	{
		//cout << "Making particle!" << endl;
		// make particle
		addParticle(currentTime);
		lastParticleSpawn = currentTime;
	}

	// Particle destruction phase
	for (unsigned int i = 0; i < particleVec.size(); i++)
	{
		if (currentTime - particleVec.at(i)->getStartTime() >= particleValues.particleLifetime)
		{
			particlesToDestroyVec.push_back(particleVec.at(i));
		}
	}

	destroyQueuedParticles();

	// Particle simulation phase
	// Moves each of the particles via its assigned velocity
	// and is also responsible for making particles run away from the camera. 
	glm::vec3 avoidLoc = cameraPos;
	glm::vec3 pVec;

	for (unsigned int i = 0; i < particleVec.size(); i++)
	{
		pVec = particleVec.at(i)->pRenderer->transform.position;
		float dist = glm::distance(pVec, avoidLoc);

		if (dist < avoidRadius)
		{
			// run away
			glm::vec3 dir = glm::normalize(pVec - avoidLoc);
			particleVec.at(i)->pRenderer->transform.position += dir * avoidSpeed * deltaTime;
			//float dir = 
		}

		particleVec.at(i)->pRenderer->transform.position += (particleVec.at(i)->velocityOffset * particleValues.getVelocity(deltaTime, currentTime, particleVec.at(i)->getStartTime()));

		particleVec.at(i)->updateParticle();
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

// rendering phase
void ParticleSystem::renderSystem()
{
	for (unsigned int i = 0; i < particleVec.size(); i++)
	{
		Particle* pTarget = particleVec.at(i);
		particleShader->use();
		particleShader->setVec3("color", glm::vec3(1.0f, 1.0f, 0.0f));
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

		// Generating random values
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
		// Random value generation complete. Just adding to the particle vector now

		particleVec.push_back(pParticle);

		lightSystemRef->AddPointLight(&pParticle->lightData);
	}
}

void ParticleSystem::removeParticle(Particle* pParticleToDestroy)
{
	for (unsigned int i = 0; i < particleVec.size(); i++)
	{
		Particle* pTarget = particleVec.at(i);
		if (pTarget == pParticleToDestroy)
		{
			lightSystemRef->RemovePointLight(&pTarget->lightData);
			pTarget->~Particle();
			particlePool.freeObject((Byte*)pTarget);

			particleVec.erase(particleVec.begin() + i);

			break;
		}

	}
}

void ParticleSystem::setLightingSystemRef(lightSystem::LightingSystem* system)
{
	lightSystemRef = system;
}

