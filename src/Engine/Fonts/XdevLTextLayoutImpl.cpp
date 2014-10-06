#include "XdevLTextLayoutImpl.h"
#include <Engine/Types.h>

namespace soan {

	XdevLTextLayoutImpl::XdevLTextLayoutImpl(xdl::XdevLWindowDevice* window, xdl::XdevLOpenGL330* openGL) :
		m_window(window),
		m_openGL(openGL),
		m_shaderProgram(nullptr),
		m_vertexShader(nullptr),
		m_fragmentShader(nullptr),
		m_vertexShaderFilename("resources/shaders/text_vs.glsl"),
		m_fragmentShaderFilename("resources/shaders/text_fs.glsl"),
		m_gamma(0.2f),
		m_buffer(0.4f),
		m_dft(1),
		m_scale(1.0f),
		m_rescale(1.0f),
		m_dpi(42.0f),
		m_effectNumber(1),
		m_staticVertexListUploaded(xdl::xdl_false) {
		m_shadowOffset[0] = 0.0f;
		m_shadowOffset[1] = 0.0f;
	}
	
	void XdevLTextLayoutImpl::setShadowOffset(xdl::xdl_float xOffset, xdl::xdl_float yOffset) {
		m_shadowOffset[0] = xOffset;
		m_shadowOffset[1] = yOffset;		
	}
	
	xdl::xdl_int XdevLTextLayoutImpl::init(XdevLFont* font) {
		m_font = font;

		m_openGL->createShaderProgram(&m_shaderProgram);
		m_openGL->createVertexShader(&m_vertexShader);
		m_openGL->createFragmentShader(&m_fragmentShader);
		m_openGL->createTexture(&m_texture);

		if(m_vertexShader->compileFromFile(m_vertexShaderFilename.c_str()) != xdl::ERR_OK) {
			std::cerr << "XdevLFont2D::init: Could not create vertex shader." << std::endl;
			return xdl::ERR_ERROR;
		}
		if(m_fragmentShader->compileFromFile(m_fragmentShaderFilename.c_str()) != xdl::ERR_OK) {
			std::cerr << "XdevLFont2D::init: Could not create fragment shader." << std::endl;
			return xdl::ERR_ERROR;
		}

		m_shaderProgram->attach(m_vertexShader);
		m_shaderProgram->attach(m_fragmentShader);
		m_shaderProgram->link();


		m_projViewModelMatrix	= m_shaderProgram->getUniformLocation("projViewModelMatrix");
		m_texture0 						= m_shaderProgram->getUniformLocation("texture0");
		m_color								= m_shaderProgram->getUniformLocation("color0");
		m_gammaid							= m_shaderProgram->getUniformLocation("gamma");
		m_bufferid						= m_shaderProgram->getUniformLocation("buffer");
		m_dftid								= m_shaderProgram->getUniformLocation("dft");
		m_effectid						= m_shaderProgram->getUniformLocation("effect");
		m_shadowOffsetid			= m_shaderProgram->getUniformLocation("shadowOffset");
		
		m_vd = new xdl::XdevLVertexDeclaration();
		m_vd->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);
		m_vd->add(4, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_COLOR);
		m_vd->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_TEXTURE_COORD);


		m_openGL->createVertexBuffer(&m_vertexBuffer);
		m_vertexBuffer->init();
		m_openGL->createVertexArray(&m_vertexArray);
		m_vertexArray->init(m_vertexBuffer, m_vd);


		m_openGL->createVertexBuffer(&m_staticVertexBuffer);
		m_staticVertexBuffer->init();
		m_openGL->createVertexArray(&m_staticVertexArray);
		m_staticVertexArray->init(m_staticVertexBuffer, m_vd);

		//
		// Set two pixel shadow offset.
		//
		m_shadowOffset[0] = -3.0f*(1.0f/(xdl::xdl_float)font->getTexture(0)->getWidth());
		m_shadowOffset[1] = 3.0f*(1.0f/(xdl::xdl_float)font->getTexture(0)->getHeight());
		
		return xdl::ERR_OK;
	}

	void  XdevLTextLayoutImpl::addDynamicText(const std::wstring&  text, xdl::xdl_float x, xdl::xdl_float y) {
		XdevLTextLayoutText info;
		info.text = text;
		info.x = x;
		info.y = y;
		info.color[0] = m_currentColor[0];
		info.color[1] = m_currentColor[1];
		info.color[2] = m_currentColor[2];
		info.color[3] = m_currentColor[3];
		info.scale 		= m_rescale;
		m_textList.push_back(info);

	}


	void XdevLTextLayoutImpl::render() {
		//
		// Do we have anything to draw?
		//
		if(m_textList.size() == 0) {
			return;
		}


		m_openGL->setActiveShaderProgram(m_shaderProgram);

		//
		// Set Signed Distance Field shader stuff.
		//
		m_shaderProgram->setUniform(m_bufferid, 	m_buffer);
		m_shaderProgram->setUniform(m_gammaid,  	m_gamma);
		m_shaderProgram->setUniformi(m_dftid, 		m_dft);
		m_shaderProgram->setUniformi(m_effectid, 	m_effectNumber);
		m_shaderProgram->setUniform2v(m_shadowOffsetid,1 , m_shadowOffset);

		//
		// Set transformation stuff.
		//
//		tmath::mat4 proj,trans, m;
//		tmath::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f, proj);
//		tmath::translate(0.0f, 0.0f, -1.8f, trans);
//		tmath::roty(56.0f, m);
//		m_shaderProgram->setUniformMatrix4(m_projViewModelMatrix, 1, proj*trans*m);


		//
		// Activate the neccessary texture.
		//
		m_shaderProgram->setUniformi(m_texture0, 0);


		//
		// TODO Replace this part using XdevL.
		//
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);



		//
		// Create the vertices for the dynamic text.
		//
		layoutVertexBuffer(m_textList, m_dynamicVertexMap);

		//
		// Draw all glyphs.
		//
		for(auto& ib : m_dynamicVertexMap) {

			//
			// Activate the corresponding texture.
			//
			m_font->getTexture(ib.first)->activate(0);


			//
			// Upload the vertex data to the GPU.
			//
			m_vertexBuffer->lock();
			m_vertexBuffer->upload((xdl::xdl_uint8*)ib.second.data(), ib.second.size()* sizeof(XdevLGlyphVertex));
			m_vertexBuffer->unlock();

			//
			// Draw everything.
			//
			m_openGL->setActiveVertexArray(m_vertexArray);
			m_openGL->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, ib.second.size());


//			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//			m_font->getTexture(ib->first)->deactivate();
//			m_openGL->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, ib->second.size());
//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		}




		glDisable(GL_BLEND);

		//
		// Clear the dynamic text list.
		//
		m_textList.clear();
		m_dynamicVertexMap.clear();
	}

	void XdevLTextLayoutImpl::layoutVertexBuffer(std::vector<XdevLTextLayoutText>& textList, std::map<xdl::xdl_uint, std::vector<XdevLGlyphVertex> >& vertexList) {

		//
		// Go through all text lines.
		//
		for(auto& textLine : textList) {

			//
			// Set cursor position.
			//
			xdl::xdl_float pen_x = textLine.x;
			xdl::xdl_float pen_y = textLine.y;

			//
			// Set the current scale for the whole line.
			// TODO Shall we do that for each letter?
			//
			m_rescale = textLine.scale;


			xdl::xdl_float unit_x 	=  m_rescale * 2.0/(xdl::xdl_float)m_window->getWidth();
			xdl::xdl_float unit_y 	=  m_rescale * 2.0/(xdl::xdl_float)m_window->getHeight();

			//
			// Go through all letters.
			//

			for(auto& letter : textLine.text) {

				//
				// Get the index for the letter.
				//
				char16_t idx = letter;

				//
				// Get the letters corrosponding glph info.
				//
				XdevLGlyphMetric glyphProperties = m_font->getGlyphMetric(idx);

				//
				// Is it a new line?
				//
				if(idx == '\n') {
					pen_x = textLine.x;
					pen_y -= unit_y * m_font->getNewLineSize();
					continue;
				}

				//
				// Or a white space?
				//
				else if(idx == ' ') {
					pen_x += unit_x * (xdl::xdl_float)(glyphProperties.horizontalLayoutAdvance);
					continue;
				}

				//
				// Prepare the vertices for 2 triangles.
				//
				// ->
				// 1-----2
				// |  /  |
				// 0-----3
				//

				XdevLGlyphVertex bottom_left, top_left, top_right, bottom_right;

				bottom_left.x = pen_x + unit_x * glyphProperties.vertices[0].x;
				bottom_left.y = pen_y - unit_y * glyphProperties.vertices[0].y;
				bottom_left.s = glyphProperties.vertices[0].s;
				bottom_left.t = glyphProperties.vertices[0].t;
				bottom_left.r = textLine.color[0];
				bottom_left.g = textLine.color[1];
				bottom_left.b = textLine.color[2];
				bottom_left.a = textLine.color[3];

				top_left.x = pen_x + unit_x * glyphProperties.vertices[1].x;
				top_left.y = pen_y + unit_y * glyphProperties.vertices[1].y;
				top_left.s = glyphProperties.vertices[1].s;
				top_left.t = glyphProperties.vertices[1].t;
				top_left.r = textLine.color[0];
				top_left.g = textLine.color[1];
				top_left.b = textLine.color[2];
				top_left.a = textLine.color[3];

				top_right.x = pen_x + unit_x * glyphProperties.vertices[2].x;
				top_right.y = pen_y + unit_y * glyphProperties.vertices[2].y;
				top_right.s = glyphProperties.vertices[2].s;
				top_right.t = glyphProperties.vertices[2].t;
				top_right.r = textLine.color[0];
				top_right.g = textLine.color[1];
				top_right.b = textLine.color[2];
				top_right.a = textLine.color[3];

				bottom_right.x = pen_x + unit_x * glyphProperties.vertices[3].x;
				bottom_right.y = pen_y - unit_y * glyphProperties.vertices[3].y;
				bottom_right.s = glyphProperties.vertices[3].s;
				bottom_right.t = glyphProperties.vertices[3].t;
				bottom_right.r = textLine.color[0];
				bottom_right.g = textLine.color[1];
				bottom_right.b = textLine.color[2];
				bottom_right.a = textLine.color[3];

				std::vector<XdevLGlyphVertex>& currentTexture = vertexList[glyphProperties.tid];

				currentTexture.push_back(bottom_left);
				currentTexture.push_back(top_left);
				currentTexture.push_back(top_right);
				currentTexture.push_back(bottom_left);
				currentTexture.push_back(top_right);
				currentTexture.push_back(bottom_right);

				pen_x += unit_x * (glyphProperties.horizontalLayoutAdvance);
			}

		}
	}

	void XdevLTextLayoutImpl::setScale(xdl::xdl_float scale) {
		m_rescale = scale;
	}

	void  XdevLTextLayoutImpl::setColor(xdl::xdl_float r, xdl::xdl_float g, xdl::xdl_float b) {
		m_currentColor[0] = r;
		m_currentColor[1] = g;
		m_currentColor[2] = b;
	}

	void XdevLTextLayoutImpl::setDPI(xdl::xdl_float dpi) {
		m_dpi = dpi;

		m_scale = m_dpi/(xdl::xdl_float)m_font->getFontSize();
	}

	void  XdevLTextLayoutImpl::setEffect(xdl::xdl_uint effectNumber) {
		m_effectNumber = effectNumber;
	}

}
