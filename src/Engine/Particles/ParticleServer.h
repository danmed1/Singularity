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

#ifndef PARTICLESERVER_H
#define PARTICLESERVER_H



#include <map>

#include "ParticleRenderer.h"

struct CParticle;
class CParticleServer;
class CParticleEmitter;

/**
	@class		CParticleServer
	@brief		Diese Klasse verwaltet mehrere CParticleEmitter
	@author		Cengiz Terzibas
	@remarks	
*/
/*
	Create a instance of the CParticle server, after that create dynamic equation for a emitter.
	After that create a generator method for the emitter. After that you can create the emitter
	\code
	CParticleServer* pserver = CParticleServer::GetInstance();
	CParticleDynamicEuler* dyn = new CParticleDynamicEuler();

	CParticleGeneratorBox* gbox = new CParticleGeneratorBox();

	tmath::vector3<float> emitterpos = tmath::vector3<float>(0.0f, 0.0f, 0.0f);
	tmath::vector3<float> emitterdir = tmath::vector3<float>(0.0f, 1.0f, 0.0f);
	CParticleEmitter* emitter = new CParticleEmitter(10, emitterpos, emitterdir, gbox);

	emitter->SetGravitationState(false);
	emitter->SetCParticleSize(pointsize);
	emitter->SetCParticleGenerator(gbox);
	emitter->ResetSystem();
	pserver->AddEmitter(emitter);	


	\endcode

	You can see that we create 10 CParticles, with a specified position and direction of the emitter. 
	Than we setting some other parameters of the emitter and register it to the CParticleserver.

*/
class CParticleServer 
{
public:
	CParticleServer();
	virtual ~CParticleServer(); 

	int	Create();
	virtual bool AddEmitter(CParticleEmitter* pEmitter);
	virtual void Update(float dT);
	virtual int Render();
	CParticleEmitter* GetEmitter(int pID);
	void SetRenderer(CParticleRenderer* pRenderer) { m_Renderer= pRenderer;}
	
	typedef std::map<int, CParticleEmitter*> CParticleEmitterMap;
protected:
	bool AddEmitter(int pID, CParticleEmitter* pEmitter);
	CParticleEmitterMap					mEmitterMap;
	CParticleRenderer*					m_Renderer;

};

#endif
