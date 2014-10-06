#include "ParticleGenerator.h"

CParticleGenerator::CParticleGenerator() :	mPos(tmath::vec3(0.0f, 0.0f, 0.0f)),
																					mClipSpace(tmath::vec3(10.0f, 10.0f, 10.0f))
{
}

CParticleGenerator::~CParticleGenerator()
{

}
