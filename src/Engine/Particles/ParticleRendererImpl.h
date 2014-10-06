#ifndef PARTICLERENDERERXGWVBUFFER_H
#define PARTICLERENDERERXGWVBUFFER_H

#include <XdevLOpenGL/XdevLOpenGL.h>
#include "ParticleRenderer.h"

struct SParticleVertex
{
	float x,y,z;
	float u,v;
	float r,g,b,a;
};

class ParticleRendererImpl : public CParticleRenderer
{
public:
	ParticleRendererImpl(xdl::XdevLOpenGL330* pRd);
	virtual ~ParticleRendererImpl();
	virtual void Render(CParticleEmitter& pEmitter);
	virtual int Create(CParticleEmitter& pEmitter);
protected:
	xdl::XdevLOpenGL330*						m_3DProcessor;
	xdl::XdevLVertexArray*					m_vertexArray;
	xdl::XdevLVertexBuffer*					m_vertexBuffer;
	xdl::XdevLVertexDeclaration*		m_vertexDeclaration;	
};

#endif

