/*
	Copyright (c) 2015 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
	
	cengiz@terzibas.de
*/

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
