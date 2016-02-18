#include "Engine/ProceduralSystem.h"

namespace soan {

	ProceduralSystem::ProceduralSystem(xdl::IPXdevLRAI rai) :
		m_rai(rai) {
	}

	std::shared_ptr<soan::Mesh> ProceduralSystem::createGrid(xdl::xdl_float size, xdl::xdl_float resolution) {

		float min = -size;
		float max = size;
		float step = resolution;

		std::cout << "Vertices: \n";
		std::vector<ProceduralSystemGridVertex> vertices;
		for(float y = min; y <= max; y+= step) {
			for(float x = min; x <= max; x+= step) {
				ProceduralSystemGridVertex v1;
				v1.x = x;
				v1.y = y;
				v1.z = 0.0f;

				v1.nx = 0.0f;
				v1.ny = 0.0f;
				v1.nz = 0.0f;

				v1.r = v1.g = v1.b = v1.a = 1.0f;
				std::cout << "(" << v1.x << "," << v1.y << ")" << std::endl;
				vertices.push_back(v1);
			}
		}

		std::cout << "Indices: \n";
		std::vector<xdl::xdl_uint16> indices;
		unsigned int width = abs( (min - max)/step);
		for(unsigned int y = 0; y < width; y++) {
			for(unsigned int x = 0; x < width; x++) {
				unsigned int lp1 = x + y*(width + 1);
				unsigned int lp2 = x + width + 1 + y*(width+1);
				unsigned int lp3 = x + 1 + width + 1 + y*(width+1);
				unsigned int lp4 = x + 1 + y*(width+1);

				std::cout << "(" << lp1 << "," << lp2 << "," << lp3 << ")\n";
				std::cout << "(" << lp3 << "," << lp4 << "," << lp1 << ")\n";

				indices.push_back(lp1);
				indices.push_back(lp2);
				indices.push_back(lp3);

				indices.push_back(lp3);
				indices.push_back(lp4);
				indices.push_back(lp1);
			}
		}


		auto vd = m_rai->createVertexDeclaration();
		vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);
		vd->add(4, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_COLOR);
		vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_NORMAL);
		vd->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_TEXTURE_COORD);

		auto vb = m_rai->createVertexBuffer();
		vb->init((xdl::xdl_uint8*)vertices.data(), vertices.size() * sizeof(ProceduralSystemGridVertex));

		auto ib = m_rai->createIndexBuffer();
		ib->init(xdl::XDEVL_BUFFER_ELEMENT_UNSIGNED_INT, (xdl::xdl_uint8*)indices.data(), indices.size() );

		auto va = m_rai->createVertexArray();
		va->init(vb, ib, vd);

		auto tmp = std::make_shared<soan::Mesh>();
		auto tmp_material = std::make_shared<soan::Material>();
		tmp_material->setUseDiffuseConst(true);
		tmp_material->setUseNormalConst(false);
		tmp_material->setUseDisplacementMap(false);
		tmp_material->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);

		tmp->setMaterial(tmp_material);
		tmp->setVertexArray(va);
		tmp->setNumberOfVertices(vertices.size());
		tmp->setNumberOfFaces(indices.size()/3);
		tmp->setPrimitive(xdl::XDEVL_PRIMITIVE_TRIANGLES);

		return tmp;
	}
}
