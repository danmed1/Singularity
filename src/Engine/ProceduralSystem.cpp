#include "Engine/ProceduralSystem.h"

namespace soan {

	ProceduralSystem::ProceduralSystem(xdl::IPXdevLRAI rai) :
		m_rai(rai) {
	}

	std::shared_ptr<soan::Mesh> ProceduralSystem::createGrid(xdl::xdl_float width, xdl::xdl_float length, xdl::xdl_float height, xdl::xdl_float gap) {

		std::vector<ProceduralSystemGridVertex> vertices;
		auto w2 = width * 0.5;
		auto l2 = length * 0.5;
		auto h2 = height * 0.5;

		for(auto w = 0.0f; w < width; w+= gap) {
			for(auto l = 0.0f; l < height; l+= gap) {
				ProceduralSystemGridVertex v1;

				// Set color.
				v1.r = 0.7f;
				v1.g = 0.7f;
				v1.b = 0.7f;
				v1.a = 1.0f;

				// Set position.
				v1.x = w - w2;
				v1.y = 0.0f;
				v1.z = l - l2;

				vertices.push_back(std::move(v1));
			}
		}

		std::vector<xdl::xdl_uint16> m_indices;
		xdl::xdl_int count = 0;
		xdl::xdl_uint rx = width/gap;
		xdl::xdl_uint ry = height/gap;

		for(auto i = 0; i < rx - 1; i++) {
			for(auto j = 0; j < ry - 1; j++) {
				m_indices.push_back(i * ry + j);
				m_indices.push_back(i * ry + j + 1);
				m_indices.push_back((i + 1) * ry + j);
				m_indices.push_back((i + 1) * ry + j);
				m_indices.push_back(i * ry + j +1);
				m_indices.push_back(( i + 1) * ry + j + 1);
			}
		}


		auto vd = m_rai->createVertexDeclaration();
		vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);
		vd->add(4, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_COLOR);

		auto vb = m_rai->createVertexBuffer();
		vb->init((xdl::xdl_uint8*)vertices.data(), vertices.size() * sizeof(ProceduralSystemGridVertex));

		auto ib = m_rai->createIndexBuffer();
		ib->init(xdl::XDEVL_BUFFER_ELEMENT_INT, (xdl::xdl_uint8*)m_indices.data(), m_indices.size() );

		auto va = m_rai->createVertexArray();
		va->init(vb, ib, vd);

		auto tmp = std::make_shared<soan::Mesh>();
		auto tmp_material = std::make_shared<soan::Material>();
		tmp_material->setUseDiffuseConst(true);
		tmp_material->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
		
		tmp->setMaterial(tmp_material);
		tmp->setVertexArray(va);
		tmp->setNumberOfVertices(vertices.size());
		tmp->setPrimitive(xdl::XDEVL_PRIMITIVE_LINES);

		return tmp;
	}
}
