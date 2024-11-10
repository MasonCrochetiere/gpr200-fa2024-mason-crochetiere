#include "Particle.h"
using namespace std;

Particle::Particle(Mesh particleMesh, Shader &particleShader, float sTime)
{
	pRenderer = new MeshRenderer(particleMesh, Transform(), &particleShader);
	startTime = sTime;

	pRenderer->transform.position.x = RandomRange(-4, 4);
	pRenderer->transform.position.y = RandomRange(-4, 4);
	pRenderer->transform.position.z = RandomRange(-0.5, -10);

	pRenderer->transform.rotation.x = RandomRange(0.5, 4);
	pRenderer->transform.rotation.y = RandomRange(0.5, 4);
	pRenderer->transform.rotation.z = RandomRange(0.5, 4);

	pRenderer->transform.scale.x = RandomRange(0.1, 1.5);
	pRenderer->transform.scale.y = RandomRange(0.1, 1.5);
	pRenderer->transform.scale.z = RandomRange(0.1, 1.5);

	cout << "I am particle. scale is x " << pRenderer->transform.scale.x << " " << pRenderer->transform.scale.y << " " << pRenderer->transform.scale.z << endl;
}

Particle::~Particle()
{
	delete pRenderer;
}

void Particle::renderParticle()
{
	pRenderer->modelAndDraw();
}