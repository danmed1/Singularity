
#include "ParticleDynamicEuler.h"

CParticleDynamicEuler::CParticleDynamicEuler() 
{

}
CParticleDynamicEuler::~CParticleDynamicEuler()
{

}

void CParticleDynamicEuler::UpdateParticle(CParticle& pCParticle, float dT)
{
	pCParticle.age += dT;
	pCParticle.position.x = pCParticle.position.x + pCParticle.v.x*dT;
	pCParticle.position.y = pCParticle.position.y + pCParticle.v.y*dT;
	pCParticle.position.z = pCParticle.position.z + pCParticle.v.z*dT;
	pCParticle.a = 1.0f - (1.0f/pCParticle.lt)*pCParticle.age ;
}