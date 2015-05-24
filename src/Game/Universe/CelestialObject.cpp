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

#include "CelestialObject.h"

namespace soan {

	namespace game {

		CelestialObject::CelestialObject()  {
			
		}

		CelestialObject::~CelestialObject() {

		}
		
		void CelestialObject::add(float mass, const std::shared_ptr<Model>& model) {
			assert(model && "Model was not specified");
			m_mass = mass;

			Renderable::setModel(model);
		}

		float& CelestialObject::getMass() {
			return m_mass;
		}
		
	
		void CelestialObject::setMass(float mass) {
			m_mass = mass;
		}
		
	
		bool CelestialObject::update(xdl::xdl_uint64 timeStep) {
			
			return Actor::update(timeStep);
			

		}

	}

}
