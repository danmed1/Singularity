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
