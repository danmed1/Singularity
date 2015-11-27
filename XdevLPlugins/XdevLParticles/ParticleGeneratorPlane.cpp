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
