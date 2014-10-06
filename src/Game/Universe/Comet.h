#ifndef COMET_H
#define COMET_H

#include "CelestialObject.h"

namespace soan {

	namespace game {

		/**
			@class Comet
			@brief A comet is an icy small Solar System body. (Wikipedia)
			
			When passing close to the Sun, heats up and begins to outgas, displaying a visible atmosphere or coma, and sometimes also a tail. (Wikipedia)
		*/
		class Comet : public CelestialObject {
			public:
				Comet();
				virtual ~Comet();

		};

	}

}

#endif
