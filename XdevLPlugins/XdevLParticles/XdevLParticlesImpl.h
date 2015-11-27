#ifndef XDEVL_PARTICLES_IMPL_H
#define XDEVL_PARTICLES_IMPL_H

#include <XdevLParticles/XdevLParticles.h>

#include <XdevLPluginImpl.h>

namespace xdl {

	static const XdevLString vendor {
		"Vendors Name"
	};

	static const XdevLString author {
		"Authors Name"
	};

	static const XdevLString copyright {
		"Copyright here."
	};

	static const XdevLString pluginName {
		"XdevLParticles"
	};

	static const XdevLString moduleDescription {
		"This is an empty module"
	};

	static std::vector<XdevLModuleName>	moduleNames {
		XdevLModuleName("XdevLParticles"),
	};

	class XdevLParticlesImpl : public XdevLModuleImpl<XdevLParticles> {

		public:

			XdevLParticlesImpl(XdevLModuleCreateParameter* parameter);

			virtual ~XdevLParticlesImpl();

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;

			virtual CParticleServer* getParticleServer() override;
			virtual CParticleEmitter* createParticleEmitter() override;

		private:
			CParticleServer* m_particleServer;
	};


}

#endif
