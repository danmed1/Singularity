#include "CanvasXdevLOpenGL.h"
#include "Engine/Types.h"
#include <tm/tm.h>

class text;
namespace soan {

	CanvasXdevLOpenGL::CanvasXdevLOpenGL(xdl::xdl_uint width, xdl::xdl_uint height, xdl::XdevLTextLayout* textLayoutSystem, xdl::XdevLOpenGL330* opengl) :
		m_textLayoutSystem(textLayoutSystem),
		m_opengl(opengl),
		m_linesStripVertexArray(nullptr),
		m_linesStripVertexBuffer(nullptr),
		m_linesStripVertexDeclaration(nullptr),
		m_linesStripShaderProgram(nullptr),
		m_width(width),
		m_height(height),
		m_currentColor(soan::Color(255, 255, 255, 255)) {

		m_linesStripVertexDeclaration = new xdl::XdevLVertexDeclaration();
		m_linesStripVertexDeclaration->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);
		m_linesStripVertexDeclaration->add(4, xdl::XDEVL_BUFFER_ELEMENT_UNSIGNED_BYTE, VERTEX_COLOR);

		m_opengl->createVertexBuffer(&m_linesStripVertexBuffer);
		m_linesStripVertexBuffer->init();

		m_opengl->createVertexArray(&m_linesStripVertexArray);
		m_linesStripVertexArray->init(m_linesStripVertexBuffer, m_linesStripVertexDeclaration);

		//
		// Create shader
		//

		m_opengl->createShaderProgram(&m_linesStripShaderProgram);

		xdl::XdevLVertexShader* vertexShader;
		if(m_opengl->createVertexShader(&vertexShader) == -1) {
			std::cerr << "Could not compile vertex shader." << std::endl;

		}
		vertexShader->compileFromFile("resources/shaders/ui_vs.glsl");

		xdl::XdevLFragmentShader* fragmentShader;
		if(m_opengl->createFragmentShader(&fragmentShader) == -1) {
			std::cerr << "Could not compile fragment shader." << std::endl;

		}
		fragmentShader->compileFromFile("resources/shaders/ui_fs.glsl");

		m_linesStripShaderProgram->attach(vertexShader);
		m_linesStripShaderProgram->attach(fragmentShader);
		m_linesStripShaderProgram->link();

		m_projMatrix = m_linesStripShaderProgram->getUniformLocation("projMatrix");




		//
		// Rectangle stuff
		//

		m_rectangleVertexDeclaration = new xdl::XdevLVertexDeclaration();
		m_rectangleVertexDeclaration->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);
		m_rectangleVertexDeclaration->add(4, xdl::XDEVL_BUFFER_ELEMENT_UNSIGNED_BYTE, VERTEX_COLOR);

		m_opengl->createVertexBuffer(&m_rectangleVertexBuffer);
		m_rectangleVertexBuffer->init();

		m_opengl->createVertexArray(&m_rectangleVertexArray);
		m_rectangleVertexArray->init(m_rectangleVertexBuffer, m_rectangleVertexDeclaration);


	}

	CanvasXdevLOpenGL::~CanvasXdevLOpenGL() {
		delete m_linesStripVertexDeclaration;
	}


	void CanvasXdevLOpenGL::drawLine(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) {
		CanvasLineVertex v1, v2;
		v1.x = x1;
		v1.y = y1;
		v1.r = m_currentColor.r;
		v1.g = m_currentColor.g;
		v1.b = m_currentColor.b;
		v1.a = m_currentColor.a;

		v2.x = x2;
		v2.y = y2;
		v2.r = m_currentColor.r;
		v2.g = m_currentColor.g;
		v2.b = m_currentColor.b;
		v2.a = m_currentColor.a;

		m_linesStripVertexList.push_back(v1);
		m_linesStripVertexList.push_back(v2);

	}

	void CanvasXdevLOpenGL::drawRect(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) {
		CanvasLineVertex v1, v2, v3, v4;
		v1.x = x1;
		v1.y = y1;
		v1.r = m_currentColor.r;
		v1.g = m_currentColor.g;
		v1.b = m_currentColor.b;
		v1.a = m_currentColor.a;

		v2.x = x1;
		v2.y = y2;
		v2.r = m_currentColor.r;
		v2.g = m_currentColor.g;
		v2.b = m_currentColor.b;
		v2.a = m_currentColor.a;

		v3.x = x2;
		v3.y = y2;
		v3.r = m_currentColor.r;
		v3.g = m_currentColor.g;
		v3.b = m_currentColor.b;
		v3.a = m_currentColor.a;

		v4.x = x2;
		v4.y = y1;
		v4.r = m_currentColor.r;
		v4.g = m_currentColor.g;
		v4.b = m_currentColor.b;
		v4.a = m_currentColor.a;

		m_rectangleVertexList.push_back(v1);
		m_rectangleVertexList.push_back(v2);
		m_rectangleVertexList.push_back(v3);

		m_rectangleVertexList.push_back(v1);
		m_rectangleVertexList.push_back(v3);
		m_rectangleVertexList.push_back(v4);

	}

	
	void CanvasXdevLOpenGL::drawText(const std::wstring& text, xdl::xdl_float x, xdl::xdl_float y) {
		CanvasTextInfo textInfo;
		textInfo.text = text;
		textInfo.x = x;
		textInfo.y = y;
		
		m_textList.push_back(textInfo);
	}


	void CanvasXdevLOpenGL::setCurrentColor(const soan::Color& color) {
		m_currentColor = color;
	}

	void CanvasXdevLOpenGL::render() {

		tmath::mat4 projectionMatrix;
		tmath::identity(projectionMatrix);
		tmath::ortho(0.0f,
		             (xdl::xdl_float)m_width,
		             0.0f,
		             (xdl::xdl_float)m_height,
		             -1.0f,
		             1.0f, projectionMatrix);

		//
		// Draw all lines.
		//
		m_opengl->setActiveShaderProgram(m_linesStripShaderProgram);
		m_linesStripShaderProgram->setUniformMatrix4(m_projMatrix, 1, projectionMatrix);

		if(m_linesStripVertexList.size() > 0) {
			m_linesStripVertexBuffer->lock();
			m_linesStripVertexBuffer->upload((xdl::xdl_uint8*)m_linesStripVertexList.data(), m_linesStripVertexDeclaration->vertexSize() * m_linesStripVertexList.size());
			m_linesStripVertexBuffer->unlock();

			m_opengl->setActiveVertexArray(m_linesStripVertexArray);

			m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_LINES, m_linesStripVertexList.size());
			m_linesStripVertexList.clear();
		}

		//
		// Draw all rectangles.
		//
		if(m_rectangleVertexList.size() > 0) {
			m_rectangleVertexBuffer->lock();
			m_rectangleVertexBuffer->upload((xdl::xdl_uint8*)m_rectangleVertexList.data(), m_rectangleVertexDeclaration->vertexSize() * m_rectangleVertexList.size());
			m_rectangleVertexBuffer->unlock();

			m_opengl->setActiveVertexArray(m_rectangleVertexArray);

			m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, m_rectangleVertexList.size());
			m_rectangleVertexList.clear();
		}
		
		//
		// Draw all text
		//
		if(m_textList.size() > 0) {
			for(auto& text : m_textList) {
				m_textLayoutSystem->printText(text.text, text.x, text.y);
			}
			m_textList.clear();
		}
	}

}
