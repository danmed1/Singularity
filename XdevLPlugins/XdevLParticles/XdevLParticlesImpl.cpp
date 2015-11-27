#include "XdevLParticlesImpl.h"

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLPARTICLES_PLUGIN_MAJOR_VERSION,
	XDEVLPARTICLES_PLUGIN_MINOR_VERSION,
	XDEVLPARTICLES_PLUGIN_PATCH_VERSION
};

xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::moduleDescription,
	XDEVLPARTICLES_MODULE_MAJOR_VERSION,
	XDEVLPARTICLES_MODULE_MINOR_VERSION,
	XDEVLPARTICLES_MODULE_PATCH_VERSION
};

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLParticlesImpl(parameter);
		if(!obj)
			return xdl::ERR_ERROR;
		parameter->setModuleInstance(obj);
		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor() {
	return &pluginDescriptor;
}

namespace xdl {

	XdevLParticlesImpl::XdevLParticlesImpl(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLParticles> (parameter, moduleDescriptor),
		m_particleServer(nullptr) {
	}

	XdevLParticlesImpl::~XdevLParticlesImpl() {}

	xdl_int XdevLParticlesImpl::init() {
		m_particleServer = new CParticleServer();
		return ERR_OK;
	}

	xdl_int XdevLParticlesImpl::shutdown() {
		delete m_particleServer;
	}

	CParticleServer* XdevLParticlesImpl::getParticleServer() {
		return m_particleServer;
	}
	
	CParticleEmitter* XdevLParticlesImpl::createParticleEmitter() {
		CParticleEmitter* tmp = new CParticleEmitter();
		m_particleServer->AddEmitter(tmp);
		return tmp;
	}

}
