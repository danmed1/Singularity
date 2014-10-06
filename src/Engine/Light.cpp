#include "Light.h"

namespace soan {

	Light::Light(const Type& type) :
		m_type(type),
		m_intensity(3.0) {

		m_specular[0] = 1.0f;
		m_specular[1] = 1.0f;
		m_specular[2] = 1.0f;

		m_ambient[0] = 0.1f;
		m_ambient[1] = 0.1f;
		m_ambient[2] = 0.1f;
		
	}

	const Light::Type& Light::getType() const {
		return m_type;
	}

	xdl::xdl_float Light::getIntensity() {
		return m_intensity;
	}

	xdl::xdl_float* Light::getSpecular() {
		return m_specular;
	}

	xdl::xdl_float* Light::getAmbient() {
		return m_ambient;
	}

}
