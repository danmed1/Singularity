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

#ifndef PARTICLEGENERATORBOX_H
#define PARTICLEGENERATORBOX_H

#include <Particle.h>
#include <ParticleGenerator.h>

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
