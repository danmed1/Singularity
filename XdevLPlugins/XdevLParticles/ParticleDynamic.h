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

#ifndef PARTICLEDYNAMIC_H
#define PARTICLEDYNAMIC_H

#include <Particle.h>

/** 
		@class	CParticleDynamic
		@brief	Berechnet die Dynamik eines Partikels
		@author	Cengiz Terzibas
		@remarks
		Leiten sie von dieser Klasse ihre Klasse ab und 
		definieren sie die UpdateCParticle Funktion. In ihr sollte
		stehen wie sich ihr Partikel in der Zeit dT bewegt.
		
		@code
		class MyDyn : public CParticleDynamic
		{
			MyDyn() {}
			~MyDyn() {}
			virtual void UpdateCParticle(CParticle& pCParticle, float dT)
			{
				CParticle.mCurrtime += dT;
				pCParticle.mPos.x = pCParticle.mPos.x + pCParticle.mVel.x*dT;
				pCParticle.mPos.y = pCParticle.mPos.y + pCParticle.mVel.y*dT;
				pCParticle.mPos.z = pCParticle.mPos.z + pCParticle.mVel.z*dT;
				pCParticle.mA = 1.0 - (1.0f/pCParticle.mLifetime)*pCParticle.mCurrtime ;
			}
		};
		...
		
		MyDyn dyn;
		
		@endcode
		
		In diesem Beispiel werden die Partikel in Abhängigkeit ihrer Geschwindgkeit bewegt. Weiterhin
		wird ihr Alpha Wert entsprechend ihrer Lebenszeit angepasst (fuer Beldings).
		Diese Klasse wird dann als Parameter dem Emitter uebergeben
		
		@code
		CParticleEmitter->SetCParticleDynamic(&dyn);
		@endcode
		
		Wird kein CParticleDynamic dem Emitter uebergeben, dann ist das Partikelsystem
		statisch. Die Partikel haben keine Eigendynamik.
		 
*/
class CParticleDynamic
{
public:
	CParticleDynamic();
	virtual ~CParticleDynamic();
	
	virtual void UpdateParticle(CParticle& pCParticle, float dT);

protected:
};

#endif
