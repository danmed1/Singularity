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

#include "Light.h"

namespace soan {

	Light::Light(const Type& type) :
		m_type(type),
		m_intensity(2.0) {

		m_specular[0] = 1.0f;
		m_specular[1] = 1.0f;
		m_specular[2] = 1.0f;

		m_ambient[0] = 0.05f;
		m_ambient[1] = 0.05f;
		m_ambient[2] = 0.05f;

	}

	Light::~Light() {
	}

	const Light::Type& Light::getType() const {
		return m_type;
	}

	xdl::xdl_float Light::getIntensity() {
		return m_intensity;
	}

	xdl::xdl_float* Light::getSpecular() {
		return m_specular;
	}

	xdl::xdl_float* Light::getAmbient() {
		return m_ambient;
	}

}
