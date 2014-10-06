#ifndef PLANET_H
#define PLANET_H

#include "Game/Universe/UnisersalUnits.h"
#include "Game/Universe/CelestialObject.h"

namespace soan {

	namespace game {

		/**
			@class Planet
			@brief A planet is an astronomical object orbiting a star. (Wikipedia)
		*/
		class Planet : public CelestialObject {
			public:
				Planet();
				virtual ~Planet();
				void init(Radius& radius);
				Radius& getRadius();
			private:
				Radius m_radius;
		};

	}

}

#endif
