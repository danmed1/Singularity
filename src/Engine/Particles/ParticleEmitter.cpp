#include "ParticleEmitter.h"
#include "Particle.h"

void CParticleEmitter::CreateParticles(int pNumParticles)
{
	if(mNumParticles <= pNumParticles)
	{
		if(mParticleArray) delete [] mParticleArray;
		mParticleArray = new CParticle[pNumParticles];
		mNumParticles = pNumParticles;
	
		if(mPGenerator)
		{
			for(int i = 0; i < pNumParticles; i++)
			{
				mPGenerator->CreateParticle(mParticleArray[i], *this);
			}
		}
	}
	else
	{
		if(mPGenerator)
		{
			for(int i = 0; i < pNumParticles; i++)
			{
				mPGenerator->CreateParticle(mParticleArray[i], *this);
			}
		}
	}
}

void CParticleEmitter::Update(float dT)
{
	mVisibleParticles = 0;
	for(int i = 0; i < mNumParticles; i++)
	{
		if(	mParticleArray[i].lt > mParticleArray[i].age)
		{
			if(mPDynamic)
				mPDynamic->UpdateParticle(mParticleArray[i],dT);
			else
			{
				// Partikel Dynamik steht nicht zur Verfuegung
			}
			
			UpdateAcceleration(mParticleArray[i]);
			
			if(mGravitation)
			{
				mParticleArray[i].v += ((mGForce + mParticleArray[i].f )*dT)/mParticleArray[i].m;
      }
			continue;
		}
		else
		{
			if(mPGenerator)
				mPGenerator->CreateParticle(mParticleArray[i],*this);
			else
			{
				// Partikel Generator steht nicht zur Verfuegung
			}
		}
	}
}

void CParticleEmitter::UpdateAcceleration(CParticle& pParticle)
{
	
}

void CParticleEmitter::SetParticleDynamic(CParticleDynamic* pParticleDynamic)
{
	mPDynamic = pParticleDynamic;
}
void CParticleEmitter::SetParticleGenerator(CParticleGenerator* pParticleGenerator)
{
	mPGenerator = pParticleGenerator;
}

void CParticleEmitter::SetDir(const tmath::vec3& pDir) 
{
	mDir = pDir;
}

void InitRandomizer(unsigned int pSeed)
{
		srand(pSeed);
}

float GetRandomMinMax(float pMin, float pMax)
{
	// Retrieves a random float between 0.0f and 1.0f;
//	float min = std::min(pMin, pMax);
	float ran = static_cast<float>(rand()/(float)RAND_MAX);
	float tlength = abs(pMax - pMin);
	return (pMin + ran*tlength);	
}
