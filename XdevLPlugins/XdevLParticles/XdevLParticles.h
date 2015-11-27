#ifndef XDEVL_PARTICLES_H
#define XDEVL_PARTICLES_H

#include <XdevLModule.h>
#include <ParticleServer.h>

namespace xdl {

	class XdevLParticles : public XdevLModule { 

	public:

		virtual ~XdevLParticles() {};
		virtual CParticleServer* getParticleServer() = 0;
		virtual CParticleEmitter* createParticleEmitter() = 0;
	};


}

#endif
