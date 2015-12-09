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

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <Particle.h>
#include <ParticleDynamic.h>
#include <ParticleGenerator.h>

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
class CParticleEmitter {
	public:
		CParticleEmitter(int pNumParticles = 100,
		                 const tmath::vec3 pPos = tmath::vec3(0.0f, 0.0f,0.0f),
		                 const tmath::vec3 pDir = tmath::vec3(0.0f, 1.0f, 0.0f), CParticleGenerator* pPGenerator = NULL ) :
			mPos(pPos), mNumParticles(pNumParticles), mParticleSize(100.0f),mGForce(tmath::vec3(0.0f, -9.81f, 0.0f)),
			mDir(pDir),mParticleArray(NULL),mGravitation(true),
			mColPlaneNormal(tmath::vec3(0.0f, 0.0f, 1.0f)), mColPos(tmath::vec3(0.0f, 0.0f, 0.0f)),
			mPDynamic(NULL), mPGenerator(pPGenerator), mVisibleParticles(0) {

			CreateParticles(pNumParticles);

		}

		CParticleEmitter(const CParticleEmitter& pSrc) {

			mPos = pSrc.mPos;
			mDir = pSrc.mDir;
			mParticleArray = pSrc.mParticleArray;
		}
		virtual ~CParticleEmitter() {

			if(mParticleArray) delete [] mParticleArray;
			mParticleArray = NULL;
		}
		/// Set the maximum size of the emitted particle
		inline void SetParticleSize(float pSize) {
			mParticleSize = pSize;
		}
		/// Retrieve the maximum size of the emitted particle
		inline float GetParticleSize() const {
			return mParticleSize;
		}
		/// Set the position of the emitter in space
		inline void SetPos(const tmath::vec3& pPos) {
			mPos = pPos;
		}
		/// Set the direction of the emitter in space
		void SetDir(const tmath::vec3& pDir);
		/// Retrieve the position of the emitter
		inline const tmath::vec3& GetPos() const {
			return mPos;
		}
		/// Return the direction of the emitter
		inline const tmath::vec3& GetDir() const {
			return mDir;
		}
		/// Return the number of particles
		inline const int GetNumParticles() const {
			return mNumParticles;
		}
		/// Enable/Disable gravitation influence
		inline void SetGravitationState(bool pState) {
			mGravitation = pState;
		}
		/// Retrieve the gravitation influence state
		inline bool GetGravitationState() const {
			return mGravitation;
		}
		/// Set the gravitation vector
		inline void SetGravitation(const tmath::vec3& pValue) {
			mGForce = pValue;
		}
		/// Set the dynamic calculator of the particles
		void SetParticleDynamic(CParticleDynamic* pParticleDynamic);
		/// Set the particle generator
		void SetParticleGenerator(CParticleGenerator* pParticleGenerator);
		/// Create the particles
		void CreateParticles(int pNumCParticles);
		/// Update states of all particles
		void Update(float dT);
		/// Retrieves a pointer to the particle array
		const CParticle* GetArray() const {
			return mParticleArray;
		}

		CParticle& operator[](int index) {
			assert(index >= 0 || index < mNumParticles);
			return mParticleArray[index];
		}

		virtual void UpdateAcceleration(CParticle& pParticle);


		void ResetSystem() {
			CreateParticles(mNumParticles);
		}

		int GetNumVisParticle() const {
			return mVisibleParticles;
		}

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