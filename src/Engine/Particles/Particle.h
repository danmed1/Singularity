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

#ifndef PARTICLE_H
#define PARTICLE_H

#include <tm/tm.h>


/**
		@class	CParticle
		@brief	Keeps the state information of a CParticle
		@remarks



*/
struct CParticle {
	// Empty Constructor
	CParticle() :
		position(tmath::vec3(0.0f, 0.0f, 0.0f)),
		v(tmath::vec3(0.0f, 0.0f, 0.0f)),
		m(1.001f),
		lt(0.0f),
		age(0.0f),
		f(tmath::vec3(0.0f, 0.0f, 0.0f)),
		vis(false)
	{}

	/** Constructor
		@param pPos Is the initial position of the CParticle
		@param pVel Is the initial velocity of the CParticle
		@param pLTime Is the lifetime of the CParticle
		@param pCTime Is the initial age of the CParticle
	*/
	CParticle(	const tmath::vec3& pPos,
	            const tmath::vec3& pVel,
	            float pLTime,
	            float pCTime ) :
		position(pPos),
		v(pVel),
		lt(pLTime),
		age(pCTime) {
	}

	// Destructor
	~CParticle() {
	}
	/** The position of the CParticle
	*/
	tmath::vec3					position;
	/** The color of the CParticle
	*/
	float								r, g, b, a;
	/** The velocity of the CParticle
	*/
	tmath::vec3					v;
	/** The mass of the CParticle
	*/
	float								m;
	/** The lifetime of the CParticle
	*/
	float								lt;
	/** The age of the CParticle
	*/
	float								age;
	/** The force acting on the CParticle
	*/
	tmath::vec3					f;
	/** Is the CParticle visible
	*/
	bool								vis;
};

#endif
