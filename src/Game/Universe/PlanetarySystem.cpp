#include "PlanetarySystem.h"

namespace soan {
	
	namespace game {
		
		PlanetarySystem::PlanetarySystem() {
			
		}
		
		PlanetarySystem::~PlanetarySystem() {
			
		}
		
		void PlanetarySystem::setName(const std::string& name) {
			m_name = name;
		}
		
		std::string& PlanetarySystem::getName() {
			return m_name;
		}
		
	}
	
}