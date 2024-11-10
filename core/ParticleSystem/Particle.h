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

	void renderParticle();

	float getStartTime() { return startTime; }

private:
	meshSystem::MeshRenderer* pRenderer;
	float startTime = 0;
};
