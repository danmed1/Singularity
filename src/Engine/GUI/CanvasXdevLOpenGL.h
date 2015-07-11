#ifndef CANVAS_XDEVL_OPENGL_h
#define CANVAS_XDEVL_OPENGL_h

#include <XdevLOpenGL/XdevLOpenGL.h>
#include <XdevLFont/XdevLTextLayout.h>

#include "Engine/GUI/Canvas.h"

#include <vector>

namespace soan {

	struct CanvasLineVertex {
		CanvasLineVertex() :
			x(0.0f),
			y(0.0f),
			color(0, 0, 0, 1.0f)
			{

		}
		xdl::xdl_float x;
		xdl::xdl_float y;

		soan::Color color;
	};

	struct CanvasTextInfo {
		CanvasTextInfo() :
			x(0),
			y(0),
			color(0, 0, 0, 1.0f)
		{}
		std::wstring text;
		xdl::xdl_float x;
		xdl::xdl_float y;
		soan::Color color;
	};

	class CanvasXdevLOpenGL : public Canvas {
	public:
		CanvasXdevLOpenGL(xdl::xdl_uint width, xdl::xdl_uint height, xdl::XdevLTextLayout* textLayoutSystem, xdl::XdevLOpenGL330* opengl);
		virtual ~CanvasXdevLOpenGL();

		virtual void setCurrentColor(const soan::Color& color);
		virtual void drawLine(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2);
		virtual void drawRect(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2);
		virtual void drawText(const std::wstring& text, xdl::xdl_float x, xdl::xdl_float y);
		virtual void render();
	private:
		xdl::XdevLTextLayout*			m_textLayoutSystem;
		xdl::XdevLOpenGL330*			m_opengl;
		xdl::XdevLVertexArray*			m_linesStripVertexArray;
		xdl::XdevLVertexBuffer*			m_linesStripVertexBuffer;
		xdl::XdevLVertexDeclaration*	m_linesStripVertexDeclaration;

		xdl::XdevLVertexArray*			m_rectangleVertexArray;
		xdl::XdevLVertexBuffer*			m_rectangleVertexBuffer;
		xdl::XdevLVertexDeclaration*	m_rectangleVertexDeclaration;

		xdl::XdevLShaderProgram*		m_linesStripShaderProgram;
		xdl::xdl_uint					m_projMatrix;

		xdl::xdl_uint 					m_width;
		xdl::xdl_uint					m_height;
		std::vector<CanvasLineVertex> 	m_linesStripVertexList;
		std::vector<CanvasLineVertex> 	m_rectangleVertexList;
		std::vector<CanvasTextInfo>		m_textList;
		soan::Color						m_currentColor;
	};

}

#endif // CANVAS_XDEVL_OPENGL_h
