#include "ParticleGeneratorSphere.h"
#include "ParticleEmitter.h"

void CParticleGeneratorSphere::CreateParticle(CParticle& pCParticle, CParticleEmitter& pEmitter)
{
	tmath::vec3 v;
	switch(mType)
	{
		case HULL:
		{
			float phi		 = GetRandomMinMax((float)-M_PI, M_PI);
			float theta	 = GetRandomMinMax((float)-M_PI*0.5f,(float) M_PI*0.5f);
			
			v.x = mRadius*cos(phi)*cos(theta);
			v.y = mRadius*sin(phi)*cos(theta);
			v.z = mRadius*sin(theta);
		}
		break;
		case SPHERE:
		{

			float phi		 = GetRandomMinMax((float)-M_PI, (float)M_PI);
			float theta	 = GetRandomMinMax((float)-M_PI*0.5f,(float) M_PI*0.5f);
			float radius = GetRandomMinMax(0.01f, mRadius);
			
			v.x = radius*cos(phi)*cos(theta);
			v.y = radius*sin(phi)*cos(theta);
			v.z = radius*sin(theta);
			
		}
		break;		
	}	

	pCParticle.position = pEmitter.GetPos() + v;
	tmath::normalize(v);
	pCParticle.v = v*0.1f;
	pCParticle.lt = GetRandomMinMax(m_MinLifeTime ,m_MaxLifeTime);
	pCParticle.age = 0.0f;
	pCParticle.r = 0.7f;
	pCParticle.g = 0.4f;
	pCParticle.b = 0.2f;
	pCParticle.a = 1.1f;	
	pCParticle.m = 1.1f;

}