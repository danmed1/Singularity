#ifndef PARTICLEDYNAMICEULER_H
#define PARTICLEDYNAMICEULER_H

#include "Particle.h"
#include "ParticleDynamic.h"

class CParticleDynamicEuler : public CParticleDynamic
{
public:
	CParticleDynamicEuler();
	virtual ~CParticleDynamicEuler();
	
	virtual void UpdateParticle(CParticle& pCParticle, float dT);
protected:
};

#endif
