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

#ifndef PARTICLEGENERATORSPHERE_H
#define PARTICLEGENERATORSPHERE_H

#include <Particle.h>
#include <ParticleGenerator.h>


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
