#include "ParticleRendererImpl.h"

ParticleRendererImpl::ParticleRendererImpl(xdl::XdevLOpenGL330* pRd) {
	m_3DProcessor = pRd;
}

ParticleRendererImpl::~ParticleRendererImpl() {
}

int ParticleRendererImpl::Create(CParticleEmitter& pEmitter) {

	static const xdl::xdl_float g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};

	m_3DProcessor->createVertexArray(&m_vertexArray);

	m_3DProcessor->createVertexBuffer(&m_vertexBuffer);
	m_vertexBuffer->init(NULL, sizeof(SParticleVertex)*pEmitter.GetNumParticles()*3*2);

	m_vertexDeclaration = new xdl::XdevLVertexDeclaration();

	m_vertexDeclaration->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 0);
	m_vertexDeclaration->add(4, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 4);
	m_vertexDeclaration->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 9);

	m_vertexArray->init(m_vertexBuffer, m_vertexDeclaration);

	return 0;
}
void ParticleRendererImpl::Render(CParticleEmitter& pEmitter) {
//		m_RD->SetMatrixMode(xgw::PROJECTIONMATRIX);
//		m_RD->PushMatrix();
//		m_RD->SetMatrixMode(xgw::MODELVIEWMATRIX);
//		m_RD->PushMatrix();
//		m_RD->SetDepthState(false);
//		m_RD->SetBlendState(true);
//	//	m_RD->SetBlendFunc(xgw::BLEND_ONE_MINUS_DST_COLOR, xgw::BLEND_ONE_MINUS_SRC_COLOR);
//		m_RD->SetBlendFunc(xgw::BLEND_SRC_ALPHA, xgw::BLEND_ONE);
//		m_RD->SetVertexDeclaration(m_Decl);
	m_vertexBuffer->lock();
	SParticleVertex* buffer = (SParticleVertex*)m_vertexBuffer->map(xdl::XDEVL_BUFFER_WRITE_ONLY);

	float mat[16];
	const CParticle* particles = pEmitter.GetArray();
	tmath::vec3 vRight(mat[0], mat[4], mat[8]);
	tmath::vec3 vUp(mat[1], mat[5], mat[9]);
	tmath::vec3 vPoint0;
	tmath::vec3 vPoint1;
	tmath::vec3 vPoint2;
	tmath::vec3 vPoint3;
	tmath::vec3 vCenter;

	float fAdjustedSize = pEmitter.GetParticleSize()/300.0f;
	int idx = 0;
	int numparticle = 0;

	int numparticles = pEmitter.GetNumParticles();
	for(int a = 0; a < numparticles; a++) {

		vCenter.x = particles[a].pos.x;
		vCenter.y = particles[a].pos.y;
		vCenter.z = particles[a].pos.z;

		vPoint0 = vCenter + ((-vRight - vUp) * fAdjustedSize);
		vPoint1 = vCenter + ((vRight - vUp) * fAdjustedSize);
		vPoint2 = vCenter + ((vRight + vUp) * fAdjustedSize);
		vPoint3 = vCenter + ((-vRight + vUp) * fAdjustedSize);

		buffer[idx + 0].x = vPoint0.x;
		buffer[idx + 0].y = vPoint0.y;
		buffer[idx + 0].z = vPoint0.z;
		buffer[idx + 0].u = 0.0f;
		buffer[idx + 0].v = 0.0f;
		buffer[idx + 0].r = particles[a].r;
		buffer[idx + 0].g = particles[a].g;
		buffer[idx + 0].b = particles[a].b;
		buffer[idx + 0].a = particles[a].a;


		buffer[idx + 1].x = vPoint1.x;
		buffer[idx + 1].y = vPoint1.y;
		buffer[idx + 1].z = vPoint1.z;
		buffer[idx + 1].u = 1.0f;
		buffer[idx + 1].v = 0.0f;
		buffer[idx + 1].r = particles[a].r;
		buffer[idx + 1].g = particles[a].g;
		buffer[idx + 1].b = particles[a].b;
		buffer[idx + 1].a = particles[a].a;

		buffer[idx + 2].x = vPoint2.x;
		buffer[idx + 2].y = vPoint2.y;
		buffer[idx + 2].z = vPoint2.z;
		buffer[idx + 2].u = 1.0f;
		buffer[idx + 2].v = 1.0f;
		buffer[idx + 2].r = particles[a].r;
		buffer[idx + 2].g = particles[a].g;
		buffer[idx + 2].b = particles[a].b;
		buffer[idx + 2].a = particles[a].a;


		buffer[idx + 3].x = vPoint2.x;
		buffer[idx + 3].y = vPoint2.y;
		buffer[idx + 3].z = vPoint2.z;
		buffer[idx + 3].u = 1.0f;
		buffer[idx + 3].v = 1.0f;
		buffer[idx + 3].r = particles[a].r;
		buffer[idx + 3].g = particles[a].g;
		buffer[idx + 3].b = particles[a].b;
		buffer[idx + 3].a = particles[a].a;

		buffer[idx + 4].x = vPoint3.x;
		buffer[idx + 4].y = vPoint3.y;
		buffer[idx + 4].z = vPoint3.z;
		buffer[idx + 4].u = 0.0f;
		buffer[idx + 4].v = 1.0f;
		buffer[idx + 4].r = particles[a].r;
		buffer[idx + 4].g = particles[a].g;
		buffer[idx + 4].b = particles[a].b;
		buffer[idx + 4].a = particles[a].a;

		buffer[idx + 5].x = vPoint0.x;
		buffer[idx + 5].y = vPoint0.y;
		buffer[idx + 5].z = vPoint0.z;
		buffer[idx + 5].u = 0.0f;
		buffer[idx + 5].v = 0.0f;
		buffer[idx + 5].r = particles[a].r;
		buffer[idx + 5].g = particles[a].g;
		buffer[idx + 5].b = particles[a].b;
		buffer[idx + 5].a = particles[a].a;

		idx+=6;

	}
	m_vertexBuffer->unlock();

	m_3DProcessor->setActiveVertexArray(m_vertexArray);
	m_3DProcessor->drawVertexArray(xdl::XDEVL_PRIMITIVE_QUADS, pEmitter.GetNumParticles()*4);

}


//case POINTSPRITE:
//{
//	CParticleEmitter* emitter =	GetEmitter(ecount);
//
//	float quadratic[] =  { 1.0f, 0.0f, 0.01f };
//	glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
//
//
//	glPointSize(emitter->GetParticleSize());
//	float fAdjustedSize = emitter->GetParticleSize() / 4.0f;
//
//
//	glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
//	glPointParameterf( GL_POINT_SIZE_MIN_ARB, 1.0f );
//	glPointParameterf( GL_POINT_SIZE_MAX_ARB, fAdjustedSize );
//
//	glBindTexture(GL_TEXTURE_2D,pointspriteid);
//	glEnable(GL_TEXTURE_2D);
//
//	glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
//	glEnable( GL_POINT_SPRITE_ARB );
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
//	glVertexPointer(3, GL_FLOAT, sizeof(Particle), (GLvoid*)emitter->GetParticleArray() );
//	glColorPointer(4, GL_FLOAT, sizeof(Particle), (void*)((float*)(emitter->GetParticleArray()) + 3));
//	glTranslatef(0.0, 22.0, 0.0f);
//	glDrawArrays(GL_POINTS,0,emitter->GetNumParticles());
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisable( GL_POINT_SPRITE_ARB );
//
//	break;
//}
