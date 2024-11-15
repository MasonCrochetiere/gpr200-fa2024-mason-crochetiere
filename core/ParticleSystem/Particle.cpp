#include "Particle.h"
using namespace std;

Particle::Particle(Mesh particleMesh, Shader &particleShader, float sTime)
{
	pRenderer = new MeshRenderer(particleMesh, Transform(), &particleShader);
	startTime = sTime;

	//cout << "I am particle. scale is x " << pRenderer->transform.scale.x << " " << pRenderer->transform.scale.y << " " << pRenderer->transform.scale.z << endl;
}

Particle::~Particle()
{
	delete pRenderer;
}

void Particle::renderParticle()
{
	pRenderer->modelAndDraw();
}