#ifndef CANVAS_XDEVL_OPENGL_h
#define CANVAS_XDEVL_OPENGL_h

#include <XdevLOpenGL/XdevLOpenGL.h>

#include "Engine/GUI/Canvas.h"

#include <vector>

namespace soan {

	struct CanvasLineVertex {
		CanvasLineVertex() :
			x(0.0f),
			y(0.0f),
			r(0.0f),
			g(0.0f),
			b(0.0f),
			a(1.0f) {

		}
		xdl::xdl_float x;
		xdl::xdl_float y;

		xdl::xdl_uint8 r;
		xdl::xdl_uint8 g;
		xdl::xdl_uint8 b;
		xdl::xdl_uint8 a;

	};

	class CanvasXdevLOpenGL : public Canvas {
		public:
			CanvasXdevLOpenGL(xdl::XdevLOpenGL330* opengl);
			virtual ~CanvasXdevLOpenGL();

			virtual void setCurrentColor(const soan::Color& color);
			virtual void drawLine(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2);
			virtual void drawRect(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2);
			virtual void render();
		private:
			xdl::XdevLOpenGL330*			m_opengl;
			xdl::XdevLVertexArray*			m_linesVertexArray;
			xdl::XdevLVertexBuffer*			m_linesVertexBuffer;
			xdl::XdevLVertexDeclaration*	m_linesVertexDeclaration;

			xdl::XdevLShaderProgram*		m_linesShaderProgram;
			xdl::xdl_uint					m_projMatrix;

			std::vector<CanvasLineVertex> m_lineVertexList;
	};

}

#endif // CANVAS_XDEVL_OPENGL_h
