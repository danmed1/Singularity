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