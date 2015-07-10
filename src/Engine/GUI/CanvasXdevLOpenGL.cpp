#include "CanvasXdevLOpenGL.h"
#include "Engine/Types.h"
#include <tm/tm.h>

namespace soan {

	CanvasXdevLOpenGL::CanvasXdevLOpenGL(xdl::XdevLOpenGL330* opengl) :
		m_opengl(opengl),
		m_linesVertexArray(nullptr),
		m_linesVertexBuffer(nullptr),
		m_linesVertexDeclaration(nullptr),
		m_linesShaderProgram(nullptr) {

		m_linesVertexDeclaration = new xdl::XdevLVertexDeclaration();
		m_linesVertexDeclaration->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);
		m_linesVertexDeclaration->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_NORMAL);
		m_linesVertexDeclaration->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_COLOR);

		m_opengl->createVertexBuffer(&m_linesVertexBuffer);
		m_linesVertexBuffer->init();

		m_opengl->createVertexArray(&m_linesVertexArray);
		m_linesVertexArray->init(m_linesVertexBuffer, m_linesVertexDeclaration);

		//
		// Create shader
		//

		m_opengl->createShaderProgram(&m_linesShaderProgram);

		xdl::XdevLVertexShader* vertexShader;
		if(m_opengl->createVertexShader(&vertexShader) == -1) {
			std::cerr << "Could not compile vertex shader." << std::endl;

		}
		vertexShader->compileFromFile("resources/shaders/debug_vs.glsl");

		xdl::XdevLFragmentShader* fragmentShader;
		if(m_opengl->createFragmentShader(&fragmentShader) == -1) {
			std::cerr << "Could not compile fragment shader." << std::endl;

		}
		fragmentShader->compileFromFile("resources/shaders/debug_fs.glsl");

		m_linesShaderProgram->attach(vertexShader);
		m_linesShaderProgram->attach(fragmentShader);
		m_linesShaderProgram->link();

		m_projMatrix = m_linesShaderProgram->getUniformLocation("projMatrix");

	}

	CanvasXdevLOpenGL::~CanvasXdevLOpenGL() {
		delete m_linesVertexDeclaration;
	}


	void CanvasXdevLOpenGL::drawLine(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) {
		CanvasLineVertex v1, v2;
		v1.x = x1;
		v1.y = y1;
		v1.r = 1.0f;
		v1.g = 1.0f;
		v1.b = 1.0f;
		v1.a = 1.0f;

		v2.x = x2;
		v2.y = y2;
		v2.r = 1.0f;
		v2.g = 1.0f;
		v2.b = 1.0f;
		v2.a = 1.0f;

		m_lineVertexList.push_back(v1);
		m_lineVertexList.push_back(v2);
	}

	void CanvasXdevLOpenGL::drawRect(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) {
	}

	void CanvasXdevLOpenGL::setCurrentColor(const soan::Color& color) {
	}

	void CanvasXdevLOpenGL::render() {
		
		tmath::mat4 projectionMatrix;
		tmath::identity(projectionMatrix);
		
		
		//
		// Draw all lines.
		//
		m_opengl->setActiveShaderProgram(m_linesShaderProgram);
		m_linesShaderProgram->setUniformMatrix4(m_projMatrix, 1, projectionMatrix);
		
		if(m_lineVertexList.size() > 0) {
			m_linesVertexBuffer->lock();
			m_linesVertexBuffer->upload((xdl::xdl_uint8*)m_lineVertexList.data(), m_linesVertexDeclaration->vertexSize() * m_lineVertexList.size());
			m_linesVertexBuffer->unlock();

			m_opengl->setActiveVertexArray(m_linesVertexArray);

			m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_LINES, m_lineVertexList.size());
			m_lineVertexList.clear();
		}
	}
}
