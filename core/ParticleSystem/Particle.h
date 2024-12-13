#pragma once

// This particle class was created by Mason Crochetiere
// it pulls from Dean Lawson's library to make it function within the Object Pool it is used in

#include "../DeanLib/include/DeanLibDefines.h"
#include "../DeanLib/include/CircularQueue.h"
#include "../DeanLib/include/Trackable.h"

#include "MemoryPool.h"
#include <vector>
#include "../MeshSystem/MeshRenderer.h"

#include "../Lighting/LightingSystem.h"


class Particle : public Trackable
{
public:
	Particle(meshSystem::Mesh particleMesh, Shader &particleShader, float sTime);
	~Particle();

	meshSystem::MeshRenderer* pRenderer;

	void renderParticle();
	void updateParticle();
	float getStartTime() { return startTime; }

	// so this vector is a value between -1 and 1 for each xyz, representing this particle's offset in relation to the system.
	glm::vec3 velocityOffset;

	lightSystem::PointLight lightData;
private:
	
	

	float startTime = 0;
};
