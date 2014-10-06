#ifndef PARTICLEDYNAMIC_H
#define PARTICLEDYNAMIC_H

#include "Particle.h"

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
