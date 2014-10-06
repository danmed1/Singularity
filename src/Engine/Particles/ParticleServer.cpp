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