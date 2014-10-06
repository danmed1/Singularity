#ifndef PARTICLEGENERATORSPHERE_H
#define PARTICLEGENERATORSPHERE_H

#include "Particle.h"
#include "ParticleGenerator.h"


enum eEmitterSphereType
{
	SPHERE,
	HULL
};

class CParticleGeneratorSphere : public CParticleGenerator
{
public:
	CParticleGeneratorSphere() : mType(HULL), m_MaxLifeTime(10.0f), m_MinLifeTime(4.0f) {};
	CParticleGeneratorSphere(const CParticleGeneratorSphere& pSrc)
	{ mRadius = pSrc.mRadius;}
	
	virtual ~CParticleGeneratorSphere() {};
	virtual void CreateParticle(CParticle& pCParticle, CParticleEmitter& pEmitter);
	void SetType(eEmitterSphereType pType)
	{
		mType = pType;
	}
	void SetSphereRadius(float pRadius) {mRadius = pRadius;}
	float GetSphereRadius() const {return mRadius;}
	void SetMaxLifeTime(float pMaxLifeTime) {m_MaxLifeTime = pMaxLifeTime;}
	void SetMinLifeTime(float pMinLifeTime) {m_MinLifeTime = pMinLifeTime;}
protected:
	float mRadius;
	float m_MaxLifeTime;
	float m_MinLifeTime;
	eEmitterSphereType mType;	
};

#endif
