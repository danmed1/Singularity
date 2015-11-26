#include <GL/glew.h>

#include "SingularityDebugDrawer.h"
#include "Engine/Types.h"
#include <iostream>



namespace soan {

	SingularityDebugDrawer::SingularityDebugDrawer(xdl::IPXdevLRAI openGL, xdl::XdevLTextLayout* textLayoutSystem)
		: m_openGL(openGL), m_textLayoutSystem(textLayoutSystem), m_debugMode(0) {

		vd = new xdl::XdevLVertexDeclaration();
		vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);
		vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_NORMAL);
		vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_COLOR);

		m_openGL->createVertexBuffer(&m_linesVertexBuffer);
		m_linesVertexBuffer->init();

		m_openGL->createVertexArray(&m_linesVertexArray);
		m_linesVertexArray->init(m_linesVertexBuffer, vd);

		m_openGL->createVertexBuffer(&m_pointsVertexBuffer);
		m_pointsVertexBuffer->init();

		m_openGL->createVertexArray(&m_pointsVertexArray);
		m_pointsVertexArray->init(m_pointsVertexBuffer, vd);

		m_openGL->createShaderProgram(&m_shaderProgram);

		xdl::XdevLVertexShader* vertexShader;
		if(m_openGL->createVertexShader(&vertexShader) == -1) {
			std::cerr << "Could not compile vertex shader." << std::endl;

		}
		vertexShader->compileFromFile("resources/shaders/debug_vs.glsl");

		xdl::XdevLFragmentShader* fragmentShader;
		if(m_openGL->createFragmentShader(&fragmentShader) == -1) {
			std::cerr << "Could not compile fragment shader." << std::endl;

		}
		fragmentShader->compileFromFile("resources/shaders/debug_fs.glsl");

		m_shaderProgram->attach(vertexShader);
		m_shaderProgram->attach(fragmentShader);
		m_shaderProgram->link();

		m_projMatrix = m_shaderProgram->getUniformLocation("projMatrix");
	}

	SingularityDebugDrawer::~SingularityDebugDrawer() {
	}

	void SingularityDebugDrawer::doDrawing(tmath::mat4& projectionMatrix) {

		//m_openGL->setActiveShaderProgram(m_shaderProgram);
		//m_shaderProgram->setUniformMatrix4(m_projMatrix, 1, projectionMatrix);

		//
		// Do the line drawing here.
		//
		if(m_lineVertexList.size() > 0) {
			m_linesVertexBuffer->lock();
			m_linesVertexBuffer->upload((xdl::xdl_uint8*)m_lineVertexList.data(), vd->vertexSize() * m_lineVertexList.size());
			m_linesVertexBuffer->unlock();

			m_openGL->setActiveVertexArray(m_linesVertexArray);

			m_openGL->drawVertexArray(xdl::XDEVL_PRIMITIVE_LINES, m_lineVertexList.size());
			m_lineVertexList.clear();

		}

		//
		// Do the point drawing here.
		//
		if(m_pointVertexList.size() > 0) {
			m_pointsVertexBuffer->lock();
			m_pointsVertexBuffer->upload((xdl::xdl_uint8*)m_pointVertexList.data(),vd->vertexSize() * m_pointVertexList.size());
			m_pointsVertexBuffer->unlock();

			m_openGL->setActiveVertexArray(m_pointsVertexArray);

			glPointSize(2.0f);
			m_openGL->drawVertexArray(xdl::XDEVL_PRIMITIVE_POINTS,m_pointVertexList.size());
			m_pointVertexList.clear();
			glPointSize(2.0f);

		}

		//
		// Do the triangle drawing here.
		//
		if(m_trianglesVertexList.size() > 0) {
			m_trianglesVertexBuffer->lock();
			m_trianglesVertexBuffer->upload((xdl::xdl_uint8*)m_trianglesVertexList.data(), vd->vertexSize() * m_trianglesVertexList.size());
			m_trianglesVertexBuffer->unlock();

			m_openGL->setActiveVertexArray(m_trianglesVertexArray);


			m_openGL->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES,m_trianglesVertexList.size());
			m_trianglesVertexList.clear();

		}
	}

	void	SingularityDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor) {

		btDebugVertex l0, l1;
		l0.r = fromColor.getX();
		l0.g = fromColor.getY();
		l0.b = fromColor.getZ();
		l0.x = from.getX();
		l0.y = from.getY();
		l0.z = from.getZ();

		l1.r = toColor.getX();
		l1.g = toColor.getY();
		l1.b = toColor.getZ();
		l1.x = to.getX();
		l1.y = to.getY();
		l1.z = to.getZ();

		m_lineVertexList.push_back(l0);
		m_lineVertexList.push_back(l1);

	}

	void	SingularityDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color) {
		btVector3 col(1.0f, 1.0f, 1.0f);
		drawLine(from,to,col,col);
	}

	void SingularityDebugDrawer::drawSphere(const btVector3& p, btScalar radius, const btVector3& color) {

		glTranslatef(p.getX(), p.getY(), p.getZ());

		int lats = 5;
		int longs = 5;

		int i, j;
		for(i = 0; i <= lats; i++) {
			btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar)(i - 1) / lats);
			btScalar z0  = radius*sin(lat0);
			btScalar zr0 =  radius*cos(lat0);

			btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
			btScalar z1 = radius*sin(lat1);
			btScalar zr1 = radius*cos(lat1);

			glBegin(GL_QUAD_STRIP);
			for(j = 0; j <= longs; j++) {
				btScalar lng = 2 * SIMD_PI * (btScalar)(j - 1) / longs;
				btScalar x = cos(lng);
				btScalar y = sin(lng);

				glNormal3f(x * zr0, y * zr0, z0);
				glVertex3f(x * zr0, y * zr0, z0);
				glNormal3f(x * zr1, y * zr1, z1);
				glVertex3f(x * zr1, y * zr1, z1);
			}
			glEnd();
		}

		glPopMatrix();
	}



	void	SingularityDebugDrawer::drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha) {

		const btVector3	n = btCross(b-a,c-a).normalized();


		btDebugVertex p0, p1, p2;
		p0.r = color.getX();
		p0.g = color.getY();
		p0.b = color.getZ();
		p0.x = a.getX();
		p0.y = a.getY();
		p0.z = a.getZ();
		p0.nx = n.x();
		p0.ny = n.y();
		p0.nz = n.z();

		p0.r = color.getX();
		p0.g = color.getY();
		p0.b = color.getZ();
		p0.x = b.getX();
		p0.y = b.getY();
		p0.z = b.getZ();
		p0.nx = n.x();
		p0.ny = n.y();
		p0.nz = n.z();

		p0.r = color.getX();
		p0.g = color.getY();
		p0.b = color.getZ();
		p0.x = c.getX();
		p0.y = c.getY();
		p0.z = c.getZ();
		p0.nx = n.x();
		p0.ny = n.y();
		p0.nz = n.z();

		m_trianglesVertexList.push_back(p0);
		m_trianglesVertexList.push_back(p1);
		m_trianglesVertexList.push_back(p2);
	}

	void	SingularityDebugDrawer::setDebugMode(int debugMode) {
		m_debugMode = debugMode;

	}


	int		SingularityDebugDrawer::getDebugMode() const {
		return m_debugMode;
	}


	void SingularityDebugDrawer::draw3dText(const btVector3& location,const char* textString) {
		std::cout << textString << std::endl;
	}

	void	SingularityDebugDrawer::reportErrorWarning(const char* warningString) {
		std::cout << warningString << std::endl;
	}

	void	SingularityDebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB,btScalar distance, int lifeTime, const btVector3& color) {
		// TODO This drawing causes permanent increase of the vertex buffer. Why?
		// Maybe has something todo with the lifetime.
		
//		const btVector3&from = pointOnB;
//
//		btDebugVertex p;
//		p.r = color.getX();
//		p.g = color.getY();
//		p.b = color.getZ();
//		p.x = pointOnB.getX();
//		p.y = pointOnB.getY();
//		p.z = pointOnB.getZ();
//
//		btDebugVertex to;
//		to.r = color.getX();
//		to.g = color.getY();
//		to.b = color.getZ();
//		to.x = pointOnB.getX()  + normalOnB.getX()*1.0f;
//		to.y = pointOnB.getY()  + normalOnB.getY()*1.0f;;
//		to.z = pointOnB.getZ()  + normalOnB.getZ()*1.0f;;
//
//		m_pointVertexList.push_back(p);
//
//
//		m_lineVertexList.push_back(p);
//		m_lineVertexList.push_back(to);

	}

}
