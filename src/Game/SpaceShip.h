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
