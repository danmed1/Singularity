#include "Game/SpaceShip.h"

namespace soan {

	namespace game {

		SpaceShip::SpaceShip() : 	m_maxThrustImpulse(10.5f), 
															m_maxTurnImpulse(5000.0f), 
															m_maxRollImpulse(5000.0f),
															m_maxPitchImpulse(5000.0f),
															m_currentThrustLevel(1.0f) {

		}

		SpaceShip::~SpaceShip() {

		}

		xdl::xdl_int SpaceShip::init() {

			return Actor::init();
		}
		
		void SpaceShip::setCurrentThrustLevel(xdl::xdl_float level) {
			// TODO Implement clamp of level.
			
			m_currentThrustLevel = level;
		}
		
		xdl::xdl_float SpaceShip::getCurrentThrustImpulse() {
			return m_maxThrustImpulse * m_currentThrustLevel;
		}
		
		xdl::xdl_float  SpaceShip::getMaxThrustImpulse() {
			return m_maxThrustImpulse;
		}

		xdl::xdl_float  SpaceShip::getMaxTurnImpulse() {
			return m_maxTurnImpulse;
		}
		
		xdl::xdl_float SpaceShip::getMaxRollImpulse() {
			return m_maxRollImpulse;
		}
		
		xdl::xdl_float SpaceShip::getMaxPitchImpulse() {
			return m_maxPitchImpulse;
		}

	}


}
