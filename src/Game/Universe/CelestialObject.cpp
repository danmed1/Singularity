#include "CelestialObject.h"

namespace soan {

	namespace game {

		CelestialObject::CelestialObject()  {
			
		}

		CelestialObject::~CelestialObject() {

		}
		
		void CelestialObject::add(float mass, const std::shared_ptr<Model>& model) {
			assert(model && "Model was not specified");
			m_mass = mass;

			Renderable::setModel(model);
		}

		float& CelestialObject::getMass() {
			return m_mass;
		}
		
	
		void CelestialObject::setMass(float mass) {
			m_mass = mass;
		}
		
	
		bool CelestialObject::update(xdl::xdl_uint64 timeStep) {
			
			return Actor::update(timeStep);
			

		}

	}

}
