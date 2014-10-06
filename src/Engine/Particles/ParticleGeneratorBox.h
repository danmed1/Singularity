#ifndef PARTICLEGENERATORBOX_H
#define PARTICLEGENERATORBOX_H

#include "Particle.h"
#include "ParticleGenerator.h"

class CParticleGeneratorBox : public CParticleGenerator
{
public:
	CParticleGeneratorBox() {};
	CParticleGeneratorBox(const CParticleGeneratorBox& pSrc)
	{ mSize = pSrc.mSize;}
	
	virtual ~CParticleGeneratorBox() {};

	virtual void CreateParticle(CParticle& pCParticle, CParticleEmitter& pEmitter)
	{
		tmath::vector3<float> v;
		float s = mSize*0.5f;

		v.x = GetRandomMinMax(-s, s);
		v.y = GetRandomMinMax(-s, s);
		v.z = GetRandomMinMax(-s, s);
		
		pCParticle.pos 			= pEmitter.GetPos() + v;
	//	pCParticle.Vel 			= tmath::vector3<float>(0.0f, 0.0f, 0.0f);//mDir* tmath::GetRandomMinMax(0.1f, 4.0f);
		pCParticle.v 			= pEmitter.GetDir()*GetRandomMinMax(0.1f, 4.0f);
		pCParticle.lt = GetRandomMinMax(2.0f, 10.0f);
		pCParticle.age = 0.0f;
		pCParticle.r = 0.3f;
		pCParticle.g = 0.8f;
		pCParticle.b = 0.3f;
		pCParticle.a = 1.0f;		
		pCParticle.m = GetRandomMinMax(1.0f, 1.0f);	
	}

	void SetBoxSize(float pSize) {mSize = pSize;}
	float GetBoxSize() const {return mSize;}
	
protected:
	float mSize;

};

#endif
