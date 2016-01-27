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
				ProceduralSystemGridVertex v1,v2,v3;

				// Set color.
				v1.r = 0.7f;
				v1.g = 0.7f;
				v1.b = 0.7f;
				v1.a = 1.0f;

				v2 = v1;
				v3 = v1;

				// Set position.
				v1.x = w;
				v1.y = 0.0f;
				v1.z = l;

				v2.x = w + gap;
				v2.y = 0.0f;
				v2.x = l;

				v3.x = w;
				v3.y = 0.0f;
				v3.x = (l + 1) * gap;


				vertices.push_back(std::move(v1));
				vertices.push_back(std::move(v2));
				vertices.push_back(std::move(v3));
			}
		}

		vb->init((xdl::xdl_uint8*)vertices.data(), vertices.size() * sizeof(ProceduralSystemGridVertex));
		auto tmp = std::make_shared<soan::Mesh>();
		auto tmp_material = std::make_shared<soan::Material>();
		tmp->setMaterial(tmp_material);
		tmp->setVertexArray(va);
		tmp->setNumberOfVertices(vertices.size());
		return tmp;
	}
}
