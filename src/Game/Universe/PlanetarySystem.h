#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include <string>
#include <vector>

#include "Game/Universe/UnisersalUnits.h"
#include "Game/Universe/Star.h"
#include "Game/Universe/Planet.h"
#include "Game/Universe/Satellite.h"

namespace soan {

	namespace game {

		class PlanetarySystem {
		public:

			PlanetarySystem();
			
			virtual ~PlanetarySystem();
			
			/// Sets the name of the system.
			void setName(const std::string& name);
			
			/// Returns the name of the system.
			std::string& getName();
			
		protected:
			// Holds the name of the system.
			std::string 						m_name;
			
			// Holds the stars in the system.
			std::vector<Star*> 			m_stars;
			
			// Holds the planets in the system.
			std::vector<Planet*> 		m_planets;
			
			// Holds the natural satelites in the system.
			std::vector<Satellite*> m_satelites;
			
		};

	}

}

#endif
