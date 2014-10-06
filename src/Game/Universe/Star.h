#ifndef STAR_H
#define STAR_H

#include "Game/Universe/UnisersalUnits.h"
#include "Game/Universe/CelestialObject.h"

namespace soan {
	
	namespace game {
		/**
			@class Star
			@brief A star is a massive, luminous sphere of plasma held together by its own gravity. (Wikipedia)
		*/
		class Star : public CelestialObject {
		public:
			Star();
			virtual ~Star();
			Radius& getRadius();	
		private:
			Radius m_radius;		
		};
		
	}
	
}

#endif
