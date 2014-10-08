#include "ParticleGeneratorPlane.h"
#include "ParticleEmitter.h"

CParticleGeneratorPlane::CParticleGeneratorPlane() : m_Size(1.0f)
{
}
CParticleGeneratorPlane::~CParticleGeneratorPlane()
{
}
void CParticleGeneratorPlane::CreateParticle(CParticle& pParticle, CParticleEmitter& pEmitter)
{
	tmath::vec3 pos;

	pos.x = GetRandomMinMax(-m_Size, m_Size);
	pos.y = 0.0f;
	pos.z = GetRandomMinMax(-m_Size, m_Size);


	pParticle.position 			= pEmitter.GetPos() + pos;
	pParticle.v 				= pEmitter.GetDir()*GetRandomMinMax(3.001f, 5.1f);
	pParticle.lt				= GetRandomMinMax(0.5f, 1.5f);
	pParticle.age				= 0.0f;
	pParticle.r					= 0.7f;
	pParticle.g					= 0.4f;
	pParticle.b					= 0.2f;
	pParticle.a					= 1.0f;		
	pParticle.m					= GetRandomMinMax(0.5f, 0.5f);	
}
