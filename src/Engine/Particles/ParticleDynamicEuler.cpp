
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
	pCParticle.pos.x = pCParticle.pos.x + pCParticle.v.x*dT;
	pCParticle.pos.y = pCParticle.pos.y + pCParticle.v.y*dT;
	pCParticle.pos.z = pCParticle.pos.z + pCParticle.v.z*dT;
	pCParticle.a = 1.0f - (1.0f/pCParticle.lt)*pCParticle.age ;
}