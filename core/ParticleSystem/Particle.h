#pragma once

#include "../DeanLib/include/DeanLibDefines.h"
#include "../DeanLib/include/CircularQueue.h"
#include "../DeanLib/include/Trackable.h"

#include "MemoryPool.h"
#include <vector>
#include "../MeshSystem/MeshRenderer.h"


class Particle : public Trackable
{
public:
	Particle(Mesh particleMesh, Shader &particleShader, float sTime);
	~Particle();

	meshSystem::MeshRenderer* pRenderer;

	void renderParticle();
	float getStartTime() { return startTime; }
private:
	
	float startTime = 0;

	// so this vector is a value between -1 and 1 for each xyz, representing this particle's offset in relation to the system.
	glm::vec3 velocityOffset;
};
