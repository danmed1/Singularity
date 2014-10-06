#include "ParticleRendererShaderCg.h"

char vertexshader [] =
"\
void C6E2v_particle(float4 pInitial : POSITION,\
                    float  tInitial : TEXCOORD0,\
                    float4 vInitial : TEXCOORD1,\
								    out float4 oPosition : POSITION,\
						        out float  pointSize : PSIZE,\
				            uniform float    time,\
										uniform float4   acceleration,\
										uniform float4x4 modelViewProj)\
{\
  float t = time - tInitial;\
  float4 pFinal = pInitial +\
                  vInitial * t + \
                  0.5 * acceleration * t * t;\
  oPosition = mul(modelViewProj, pFinal);\
  pointSize = -8.0 * t * t + \
               16.0 * t + \
               0.1 * pFinal.y + 1;\
  if (pointSize < 0)\
    pointSize = 0;\
}";


//CParticleRendererShaderCg::CParticleRendererShaderCg(xgw::CRenderDevice* pRd)
//{
//	m_RD = pRd;
//
//}
//
//CParticleRendererShaderCg::~CParticleRendererShaderCg()
//{
//}
//
//int CParticleRendererShaderCg::Create(CParticleEmitter& pEmitter)
//{
//	return 0;
//}
//void CParticleRendererShaderCg::Render(CParticleEmitter& pEmitter) 
//{
//}