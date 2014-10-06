
#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Engine/Particles/Particle.h"
#include "Engine/Particles/ParticleDynamic.h"
#include "Engine/Particles/ParticleGenerator.h"

#include <cassert>

/**
	@class		CParticleEmitter
	@brief		Diese Klasser erzeugt Partikel
	@author		Cengiz Terzibas
	The emitter is responsible for the creation and simulation of the particles. To catch that 
	problem he need 2 classes that helps him. First is the CParticleGenerator. This class is
	used to create particles in different ways. Maybe you want create particles on a surface 
	of an sphere or on the surface of an cube.
	To calculate the new physical states, like position, velocity etc. you have to use the
	CParticleDynamic class. This is used to update the states of all particles. 
*/
class CParticleEmitter
{
public:
	CParticleEmitter(int pNumParticles = 0,
									const tmath::vec3 pPos = tmath::vec3(0.0f, 0.0f,0.0f),
									const tmath::vec3 pDir = tmath::vec3(0.0f, 1.0f, 0.0f), CParticleGenerator* pPGenerator = NULL ) : 
									mPos(pPos), mNumParticles(pNumParticles), mParticleSize(100.0f),mGForce(tmath::vec3(0.0f, -9.81f, 0.0f)),
									mDir(pDir),mParticleArray(NULL),mGravitation(true),
									mColPlaneNormal(tmath::vec3(0.0f, 0.0f, 1.0f)), mColPos(tmath::vec3(0.0f, 0.0f, 0.0f)),
									mPDynamic(NULL), mPGenerator(pPGenerator), mVisibleParticles(0)
									{
//										std::cout << "Constructor: Emitter\n";
										CreateParticles(pNumParticles);
										
									}
	
	CParticleEmitter(const CParticleEmitter& pSrc)
	{
//		std::cout << "Copy Constructor: Emitter\n";
		mPos = pSrc.mPos;
		mDir = pSrc.mDir;
		mParticleArray = pSrc.mParticleArray;
	}
	virtual ~CParticleEmitter() 
	{
//		std::cout << "Descructor: Emitter\n";
		if(mParticleArray) delete [] mParticleArray;
		mParticleArray = NULL;
	}
	/// Set the maximum size of the emitted particle
	inline void SetParticleSize(float pSize) {mParticleSize = pSize;}
	/// Retrieve the maximum size of the emitted particle
	inline float GetParticleSize() const {return mParticleSize;}
	/// Set the position of the emitter in space
	inline void SetPos(const tmath::vec3& pPos) {mPos = pPos;}
	/// Set the direction of the emitter in space
	void SetDir(const tmath::vec3& pDir);
	/// Retrieve the position of the emitter 
	inline const tmath::vec3& GetPos() const {return mPos;}
	/// Return the direction of the emitter
	inline const tmath::vec3& GetDir() const {return mDir;}
	/// Return the number of particles
	inline const int GetNumParticles() const {return mNumParticles;}
	/// Enable/Disable gravitation influence
	inline void SetGravitationState(bool pState) {mGravitation = pState;}
	/// Retrieve the gravitation influence state
	inline bool GetGravitationState() const {return mGravitation;}
	/// Set the gravitation vector
	inline void SetGravitation(const tmath::vec3& pValue) {mGForce = pValue;}
	/// Set the dynamic calculator of the particles
	void SetParticleDynamic(CParticleDynamic* pParticleDynamic);
	/// Set the particle generator 
	void SetParticleGenerator(CParticleGenerator* pParticleGenerator);
	/// Create the particles
	void CreateParticles(int pNumCParticles);
	/// Update states of all particles
	void Update(float dT);
	/// Retrieves a pointer to the particle array
	const CParticle* GetArray() const {return mParticleArray;}
	
	CParticle& operator[](int index)
	{ 
		assert(index >= 0 || index < mNumParticles); 
		return mParticleArray[index];
	}

	virtual void UpdateAcceleration(CParticle& pParticle);


	void ResetSystem() 
	{
		CreateParticles(mNumParticles);
	}

	int GetNumVisParticle() const {return mVisibleParticles;}

protected:		
	int mNumParticles;
	tmath::vec3 mPos;
	tmath::vec3 mDir;
	CParticle*	mParticleArray;

	float mParticleSize;
	int		mVisibleParticles;
	
	bool mGravitation;
	tmath::vec3 mGForce;
	
	tmath::vec3 mColPlaneNormal;
	tmath::vec3 mColPos;
	
	CParticleDynamic* mPDynamic;
	CParticleGenerator* mPGenerator;
};


void InitRandomizer(unsigned int pSeed);
float GetRandomMinMax(float pMin, float pMax);

#endif
