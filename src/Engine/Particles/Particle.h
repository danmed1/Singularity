#ifndef PARTICLE_H
#define PARTICLE_H

#include <tm/tm.h>


/** 
		@class	CParticle
		@brief	Keeps the state information of a CParticle
		@remarks
		
		

*/
struct CParticle
{
	// Empty Constructor
	CParticle(): pos(tmath::vec3(0.0f, 0.0f, 0.0f)), 
							v(tmath::vec3(0.0f, 0.0f, 0.0f)), 
							lt(0.0f), 
							age(0.0f),m(1.001f),
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
						lt(pLTime), 
						age(pCTime), 
						pos(pPos), 
						v(pVel) 
		{
		}
		
	// Destructor
	~CParticle() 
	{
	}
	/** The position of the CParticle
	*/
	tmath::vec3	pos;
	/** The color of the CParticle
	*/
	float								r, g, b, a;
	/** The velocity of the CParticle
	*/
	tmath::vec3	v;
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
	tmath::vec3	f;
	/** Is the CParticle visible
	*/
	bool								vis;
};

#endif
