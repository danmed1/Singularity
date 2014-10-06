#include "PostProcessEffect.h"
#include "Engine/Types.h"

namespace soan {

	PostProcessEffect::PostProcessEffect(xdl::XdevLOpenGL330* opengl) : m_opengl(opengl) {
		m_outTexure.reserve(NUM_OUTPUT_TEXTURES);
		m_outTexure.resize(NUM_OUTPUT_TEXTURES);
		m_inputTexure.reserve(NUM_INPUT_TEXTURES);
		m_inputTexure.resize(NUM_INPUT_TEXTURES);


		for(auto& ib : m_outTexure) {
			ib = nullptr;
		}

		for(auto& ib : m_inputTexure) {
			ib = nullptr;
		}

	}

	PostProcessEffect::~PostProcessEffect() {
		m_opengl->destroy(m_va);
	}

	int PostProcessEffect::init(unsigned int width, unsigned height) {
		m_width = width;
		m_height = height;

		xdl::xdl_float screen_vertex [] = {
			-1.0, -1.0f,
			-1.0f, 1.0f,
			1.0f, 1.0f,

			1.0f, 1.0f,
			1.0f, -1.0f,
			-1.0f, -1.0f
		};


		xdl::XdevLVertexDeclaration* vd = new xdl::XdevLVertexDeclaration();
		vd->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);


		std::vector<xdl::xdl_uint8*> list;
		list.push_back((xdl::xdl_uint8*)screen_vertex);


		m_opengl->createVertexArray(&m_va);
		if(m_va->init(list.size(), list.data(), 6, vd) != xdl::ERR_OK) {
			return -1;
		}

		return 0;
	}

	unsigned int PostProcessEffect::getWidth() {
		return m_width;
	}

	unsigned int PostProcessEffect::getHeight() {
		return m_height;
	}

	xdl::XdevLTexture* PostProcessEffect::getInputTexture(unsigned int idx) {
		assert((idx < NUM_INPUT_TEXTURES) && "PostProcessEffect::getInputTexture: Argument not valid.");
		return m_inputTexure[idx];
	}

	xdl::XdevLTexture* PostProcessEffect::getOutputTexture(unsigned int idx) {
		return m_outTexure[idx];
	}

	void PostProcessEffect::setInputTexture(unsigned int idx, xdl::XdevLTexture* texture) {
		m_inputTexure[idx] = texture;
	}

	void PostProcessEffect::setOutputTexture(unsigned int idx, xdl::XdevLTexture* texture) {
		m_outTexure[idx] = texture;
	}
}
