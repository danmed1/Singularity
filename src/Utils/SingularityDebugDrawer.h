#ifndef GL_DEBUG_DRAWER_H
#define GL_DEBUG_DRAWER_H

#include <XdevLRAI/XdevLRAI.h>
#include <XdevLFont/XdevLFont.h>
#include <XdevLFont/XdevLFontSystem.h>
#include <XdevLFont/XdevLTextLayout.h>


#include "LinearMath/btIDebugDraw.h"
#include <tm/tm.h>
#include <vector>


namespace soan {

	struct btDebugVertex {
		float x,y,z;
		float nx, ny, nz;
		float r,g,b;
	};


	class SingularityDebugDrawer : public btIDebugDraw {
		xdl::IPXdevLRAI				m_openGL;
		xdl::IPXdevLTextLayout		m_textLayoutSystem;
		xdl::IPXdevLShaderProgram	m_shaderProgram;
		xdl::IPXdevLVertexArray		m_linesVertexArray;
		xdl::IPXdevLVertexBuffer		m_linesVertexBuffer;

		xdl::xdl_uint			m_projMatrix;

		xdl::IPXdevLVertexArray	m_pointsVertexArray;
		xdl::IPXdevLVertexBuffer	m_pointsVertexBuffer;


		xdl::IPXdevLVertexArray	m_trianglesVertexArray;
		xdl::IPXdevLVertexBuffer	m_trianglesVertexBuffer;

		int m_debugMode;
		std::vector<btDebugVertex> m_lineVertexList;
		std::vector<btDebugVertex> m_pointVertexList;
		std::vector<btDebugVertex> m_trianglesVertexList;

		xdl::IPXdevLVertexDeclaration vd;

	public:

		SingularityDebugDrawer(xdl::IPXdevLRAI openGL, xdl::IPXdevLTextLayout textLayout = nullptr);

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
