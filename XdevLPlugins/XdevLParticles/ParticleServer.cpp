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

#include "ParticleServer.h"
#include "ParticleEmitter.h"


CParticleServer::CParticleServer() 
{

}

CParticleServer::~CParticleServer() 
{
	CParticleEmitterMap::iterator i;
	i = mEmitterMap.begin();
	/*
	while(i != mEmitterMap.end())
	{
		if(i->second) delete i->second;
		i++;
	}
*/
	mEmitterMap.clear();
}

int	CParticleServer::Create()
{

	return 1;
}

void CParticleServer::Update(float dT)
{
	CParticleEmitterMap::iterator i;
	i = mEmitterMap.begin();
	while(i != mEmitterMap.end())
	{
		i->second->Update(dT);
		i++;
	}
}

bool CParticleServer::AddEmitter(CParticleEmitter* pEmitter)
{
	int s = static_cast<int>(mEmitterMap.size());
	if(AddEmitter(s,pEmitter) == false)
	{
		std::cerr << __FILE__ << " : " << __LINE__ << " : Could not create CParticle emitter!\n";
		return false;
	}
	return true;
}

CParticleEmitter* CParticleServer::GetEmitter(int pID)
{
	CParticleEmitterMap::iterator i;
	i = mEmitterMap.find(pID);
	
	if(i != mEmitterMap.end())
		return mEmitterMap[pID];
	
	return NULL;
}

bool CParticleServer::AddEmitter(int pID, CParticleEmitter* pEmitter)
{
	mEmitterMap.insert(CParticleEmitterMap::value_type(pID, pEmitter));
	if(m_Renderer)
	{
		m_Renderer->Create(*pEmitter);
	}
	return true;
}


int CParticleServer::Render()
{
	if(!m_Renderer)
		return 0;

	for(int ecount = 0; ecount != mEmitterMap.size(); ++ecount)
	{ 
		CParticleEmitter* emitter =	GetEmitter(ecount);
		m_Renderer->Render(*emitter);
	}
	return 0;
}