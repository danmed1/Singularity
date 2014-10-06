#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

#include "ParticleEmitter.h"

class CParticleRenderer
{
public:
	CParticleRenderer() {}
	virtual ~CParticleRenderer() {}
	virtual int Create(CParticleEmitter& pEmitter)
	{
		return 0;
	}
	virtual void Render(CParticleEmitter& pEmitter) 
	{
	}
	
};

#endif

