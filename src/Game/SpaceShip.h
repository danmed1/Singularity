#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "Game/Actor.h"

namespace soan {

	namespace game {

		class SpaceShip : public Actor {
			public:
				SpaceShip();
				virtual ~SpaceShip();
				
				virtual xdl::xdl_int init() final;
				
				/// Sets the thrust level value. 0.0f - 1.0f
				void setCurrentThrustLevel(xdl::xdl_float level);
				
				/// Returns the current thrust impulse.
				xdl::xdl_float getCurrentThrustImpulse();
				
				/// Returns the maximum thrust impulse.
				xdl::xdl_float getMaxThrustImpulse();
				
				/// Returns the maximum turn impulse.
				xdl::xdl_float getMaxTurnImpulse();
				
				/// Returns the maximum roll impulse.
				xdl::xdl_float getMaxRollImpulse();
				
				/// Returns the maximum pitch impulse.
				xdl::xdl_float getMaxPitchImpulse();
				
		private:
				xdl::xdl_float m_maxThrustImpulse;
				xdl::xdl_float m_maxTurnImpulse;
				xdl::xdl_float m_maxRollImpulse;
				xdl::xdl_float m_maxPitchImpulse;
				
				xdl::xdl_float m_currentThrustLevel;
		};

	}

}

#endif
