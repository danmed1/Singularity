#ifndef GL_DEBUG_DRAWER_H
#define GL_DEBUG_DRAWER_H

#include <XdevLOpenGL/XdevLOpenGL.h>
#include "LinearMath/btIDebugDraw.h"
#include "Engine/Fonts/XdevLFontImpl.h"
#include <tm/tm.h>
#include <vector>


namespace soan {

	struct btDebugVertex {
		float x,y,z;
		float nx, ny, nz;
		float r,g,b;
	};


	class SingularityDebugDrawer : public btIDebugDraw {
		xdl::XdevLOpenGL330*		m_openGL;
		xdl::XdevLFontImpl*		m_fontEngine;
		xdl::XdevLShaderProgram*	m_shaderProgram;
		xdl::XdevLVertexArray*		m_linesVertexArray;
		xdl::XdevLVertexBuffer*		m_linesVertexBuffer;

		xdl::xdl_uint			m_projMatrix;

		xdl::XdevLVertexArray*	m_pointsVertexArray;
		xdl::XdevLVertexBuffer*	m_pointsVertexBuffer;


		xdl::XdevLVertexArray*	m_trianglesVertexArray;
		xdl::XdevLVertexBuffer*	m_trianglesVertexBuffer;

		int m_debugMode;
		std::vector<btDebugVertex> m_lineVertexList;
		std::vector<btDebugVertex> m_pointVertexList;
		std::vector<btDebugVertex> m_trianglesVertexList;

		xdl::XdevLVertexDeclaration* vd;

	public:

		SingularityDebugDrawer(xdl::XdevLOpenGL330* openGL, xdl::XdevLFontImpl* fontEngine = nullptr);

		virtual ~SingularityDebugDrawer();


		virtual void doDrawing(tmath::mat4& projectionMatrix);

		virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor);

		virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);

		virtual void drawSphere(const btVector3& p, btScalar radius, const btVector3& color);

		virtual void drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);

		virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

		virtual void reportErrorWarning(const char* warningString);

		virtual void draw3dText(const btVector3& location,const char* textString);

		virtual void setDebugMode(int debugMode);

		virtual int getDebugMode() const;

	};
}
#endif//GL_DEBUG_DRAWER_H
