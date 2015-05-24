/*
	Copyright (c) 2015 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
	
	cengiz@terzibas.de
*/

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
