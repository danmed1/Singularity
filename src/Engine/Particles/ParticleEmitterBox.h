#ifndef PARTICLEEMITTERBOX_H
#define PARTICLEEMITTERBOX_H

#include "ParticleEmitter.h"

class CParticleEmitterBox : public CParticleEmitter
{
public:
	CParticleEmitterBox(int pNumCParticles = 0,
										const tmath::vector3<float> pPos = tmath::vector3<float>(0.0f, 0.0f,0.0f),
										const tmath::vector3<float> pDir = tmath::vector3<float>(0.0f, 1.0f, 0.0f),
										float pSize = 1.0f);
	virtual ~CParticleEmitterBox();
	
	virtual void CreateCParticle(CParticle& pCParticle);
	virtual void UpdateAcceleration(CParticle& pCParticle);

protected:

	float mSize;
};


#endif

