#include "Engine/ProceduralSystem.h"

namespace soan {

	ProceduralSystem::ProceduralSystem(xdl::IPXdevLRAI rai) :
		m_rai(rai) {
	}

	std::shared_ptr<soan::Mesh> ProceduralSystem::createGrid(xdl::xdl_float width, xdl::xdl_float length, xdl::xdl_float height, xdl::xdl_float gap) {
		auto vd = m_rai->createVertexDeclaration();
		vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 0);
		vd->add(4, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 1);

		auto vb = m_rai->createVertexBuffer();
		auto va = m_rai->createVertexArray();
		va->init(vb, vd);

		std::vector<ProceduralSystemGridVertex> vertices;
		auto w2 = width * 0.5;
		auto l2 = length * 0.5;
		auto h2 = height * 0.5;

		for(auto w = -w2; w < w2; w+= gap) {
			for(auto l = -l2; l < l2; l+= gap) {
				ProceduralSystemGridVertex vertex;

				// Set position.
				vertex.x = w;
				vertex.y = 0.0f;
				vertex.z = l;
				// Set color.
				vertex.r = 0.7f;
				vertex.g = 0.7f;
				vertex.b = 0.7f;
				vertex.a = 1.0f;

				vertices.push_back(std::move(vertex));
			}
		}

		vb->init((xdl::xdl_uint8*)vertices.data(), vertices.size() * sizeof(ProceduralSystemGridVertex));
		auto tmp = std::make_shared<soan::Mesh>();
		tmp->setVertexArray(va);
		tmp->setNumberOfVertices(vertices.size());
		return tmp;
	}
}
