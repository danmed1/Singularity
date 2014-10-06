#ifndef PARTICLEGENEARATORPLANE_H
#define PARTICLEGENEARATORPLANE_H

#include "ParticleGenerator.h"


class CParticleGeneratorPlane : public CParticleGenerator
{
public:
	CParticleGeneratorPlane();
	virtual ~CParticleGeneratorPlane();
	virtual void CreateParticle(CParticle& pParticle, CParticleEmitter& pEmitter);
	void SetSize(float pValue) {m_Size = pValue;}
protected:
	float m_Size;

};
#endif