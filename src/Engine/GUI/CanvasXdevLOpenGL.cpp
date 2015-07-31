#include "CanvasXdevLOpenGL.h"
#include "Engine/Types.h"
#include <tm/tm.h>

class text;
namespace soan {

	CanvasXdevLOpenGL::CanvasXdevLOpenGL(xdl::xdl_uint width, xdl::xdl_uint height, xdl::XdevLTextLayout* textLayoutSystem, xdl::XdevLOpenGL330* opengl) :
		m_textLayoutSystem(textLayoutSystem),
		m_window(nullptr),
		m_previousWindow(nullptr),
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
		m_linesStripVertexDeclaration->add(4, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_COLOR);

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
		// Rectangle Line stuff
		//

		m_rectangleLineVertexDeclaration = new xdl::XdevLVertexDeclaration();
		m_rectangleLineVertexDeclaration->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);
		m_rectangleLineVertexDeclaration->add(4, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_COLOR);

		m_opengl->createVertexBuffer(&m_rectangleLineVertexBuffer);
		m_rectangleLineVertexBuffer->init();

		m_opengl->createVertexArray(&m_rectangleLineVertexArray);
		m_rectangleLineVertexArray->init(m_rectangleLineVertexBuffer, m_rectangleLineVertexDeclaration);



		//
		// Rectangle stuff
		//

		m_rectangleVertexDeclaration = new xdl::XdevLVertexDeclaration();
		m_rectangleVertexDeclaration->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);
		m_rectangleVertexDeclaration->add(4, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_COLOR);

		m_opengl->createVertexBuffer(&m_rectangleVertexBuffer);
		m_rectangleVertexBuffer->init();

		m_opengl->createVertexArray(&m_rectangleVertexArray);
		m_rectangleVertexArray->init(m_rectangleVertexBuffer, m_rectangleVertexDeclaration);


	}

	CanvasXdevLOpenGL::~CanvasXdevLOpenGL() {
		delete m_linesStripVertexDeclaration;
	}

	void CanvasXdevLOpenGL::setCurrentWindow(xdl::IPXdevLWindow window) {
		m_previousWindow = m_window;
		m_window = window;
	}

	void CanvasXdevLOpenGL::makeCurrentWindow() {
		assert(m_window && "Window not set for the canvas.");
		m_opengl->makeCurrent(m_window);
		glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
	}

	void CanvasXdevLOpenGL::releaseCurrentWindow() {
		m_opengl->releaseCurrent();
		m_window = m_previousWindow;
	}

	xdl::IPXdevLWindow CanvasXdevLOpenGL::getWindow() {
		return m_window;
	}

	const xdl::xdl_uint& CanvasXdevLOpenGL::getWidth() const {
		return m_width;
	}

	const xdl::xdl_uint& CanvasXdevLOpenGL::getHeight() const {
		return m_height;
	}

	void CanvasXdevLOpenGL::setDimensions(xdl::xdl_uint width, xdl::xdl_uint height) {
		m_width = width;
		m_height = height;
	}

	void CanvasXdevLOpenGL::clearColorBuffer() {
		glClearColor(0.33f, 0.32f, 0.30f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void CanvasXdevLOpenGL::drawLine(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) {
		CanvasLineVertex v1, v2;
		v1.x = x1;
		v1.y = y1;
		v1.color = m_currentColor;

		v2.x = x2;
		v2.y = y2;
		v2.color = m_currentColor;

		m_linesStripVertexList.push_back(v1);
		m_linesStripVertexList.push_back(v2);

	}

	void CanvasXdevLOpenGL::drawRectLine(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) {
		CanvasLineVertex v1, v2, v3, v4;
		v1.x = x1;
		v1.y = y1;
		v1.color = m_currentColor;

		v2.x = x1;
		v2.y = y2;
		v2.color = m_currentColor;

		v3.x = x2;
		v3.y = y2;
		v3.color = m_currentColor;

		v4.x = x2;
		v4.y = y1;
		v4.color = m_currentColor;

		m_rectangleLineVertexList.push_back(v1);
		m_rectangleLineVertexList.push_back(v2);
		m_rectangleLineVertexList.push_back(v3);
		m_rectangleLineVertexList.push_back(v4);
		m_rectangleLineVertexList.push_back(v1);

	}


	void CanvasXdevLOpenGL::drawRect(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) {
		CanvasLineVertex v1, v2, v3, v4;
		v1.x = x1;
		v1.y = y1;
		v1.color = m_currentColor;

		v2.x = x1;
		v2.y = y2;
		v2.color = m_currentColor;

		v3.x = x2;
		v3.y = y2;
		v3.color = m_currentColor;

		v4.x = x2;
		v4.y = y1;
		v4.color = m_currentColor;

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
		textInfo.color = m_currentColor;

		m_textList.push_back(textInfo);
	}


	void CanvasXdevLOpenGL::setCurrentColor(const soan::Color& color) {
		m_currentColor = color;
	}

	void CanvasXdevLOpenGL::render() {
		makeCurrentWindow();

		clearColorBuffer();

		tmath::mat4 projectionMatrix;
		tmath::identity(projectionMatrix);
		tmath::ortho(0.0f,
		             (xdl::xdl_float)m_width,
		             0.0f,
		             (xdl::xdl_float)m_height,
		             -1.0f,
		             1.0f, projectionMatrix);

		m_opengl->setActiveShaderProgram(m_linesStripShaderProgram);
		m_linesStripShaderProgram->setUniformMatrix4(m_projMatrix, 1, projectionMatrix);

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
		// Draw all rectangles.
		//
		if(m_rectangleLineVertexList.size() > 0) {
			m_rectangleLineVertexBuffer->lock();
			m_rectangleLineVertexBuffer->upload((xdl::xdl_uint8*)m_rectangleLineVertexList.data(), m_rectangleLineVertexDeclaration->vertexSize() * m_rectangleLineVertexList.size());
			m_rectangleLineVertexBuffer->unlock();

			m_opengl->setActiveVertexArray(m_rectangleLineVertexArray);

			m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_LINE_STRIP, m_rectangleLineVertexList.size());
			m_rectangleLineVertexList.clear();
		}


		//
		// Draw all lines.
		//
		if(m_linesStripVertexList.size() > 0) {
			m_linesStripVertexBuffer->lock();
			m_linesStripVertexBuffer->upload((xdl::xdl_uint8*)m_linesStripVertexList.data(), m_linesStripVertexDeclaration->vertexSize() * m_linesStripVertexList.size());
			m_linesStripVertexBuffer->unlock();

			m_opengl->setActiveVertexArray(m_linesStripVertexArray);

			m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_LINES, m_linesStripVertexList.size());
			m_linesStripVertexList.clear();
		}


		//
		// Draw all text
		//
		if(m_textList.size() > 0) {
			for(auto& text : m_textList) {
				m_textLayoutSystem->setColor(text.color.r, text.color.g, text.color.b, text.color.a);
				m_textLayoutSystem->printText(text.text, text.x, text.y);
			}
			m_textList.clear();
		}

		m_opengl->swapBuffers();
		releaseCurrentWindow();
	}

}
