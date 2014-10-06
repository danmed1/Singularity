#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H

#include "Particle.h"


class CParticleEmitter;

class CParticleGenerator
{
public:
	CParticleGenerator();
	virtual ~CParticleGenerator();

	virtual void CreateParticle(CParticle& pCParticle, CParticleEmitter& pEmitter) = 0;

protected:
	tmath::vec3 mPos;
	tmath::vec3 mClipSpace;

};

#endif
