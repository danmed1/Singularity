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

#ifndef CELESTIAL_OBJECTS_H
#define CELESTIAL_OBJECTS_H


#include <Game/Actor.h>
#include <Engine/Entity/Model.h>

namespace soan {

	namespace game {

		class CelestialObject : public Actor {
		public:
			
				CelestialObject();
				
				virtual ~CelestialObject();
							
				/// Return the mass of the astroid.
				float& 				getMass();
											
				/// Sets the mass.
				void setMass(float mass);

				/// Update the objects state.
				virtual bool update(xdl::xdl_uint64 timeStep);

				/// Add a new mesh to the astroid.
				void add(float mass, const std::shared_ptr<Model>& model);
				
			protected:

				float											m_mass;
				
		};

	}

}

#endif
