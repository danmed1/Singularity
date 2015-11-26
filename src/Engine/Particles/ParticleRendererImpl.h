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

#ifndef PARTICLERENDERERXGWVBUFFER_H
#define PARTICLERENDERERXGWVBUFFER_H

#include <XdevLRAI/XdevLRAI.h>
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
	ParticleRendererImpl(xdl::IPXdevLRAI pRd);
	virtual ~ParticleRendererImpl();
	virtual void Render(CParticleEmitter& pEmitter);
	virtual int Create(CParticleEmitter& pEmitter);
protected:
	xdl::IPXdevLRAI					m_3DProcessor;
	xdl::XdevLVertexArray*			m_vertexArray;
	xdl::XdevLVertexBuffer*			m_vertexBuffer;
	xdl::XdevLVertexDeclaration*	m_vertexDeclaration;
};

#endif

