/*
	Copyright (c) 2015 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

	cengiz@terzibas.de
*/

#include "Singularity.h"
#include "Engine/Entity/Mesh.h"
#include "Engine/Types.h"
#include "Utils/AssimpToModel.h"
#include <LinearMath/btQuaternion.h>
#include <tm/tm.h>
#include <random>
#include <string>
#include <locale>
#include <memory>

Singularity::Singularity(int argc, char** argv, const char* xml_filename) throw() : xdl::XdevLApplication(argc, argv, xdl::XdevLFileName(xml_filename)),
	m_id("Singularity"),
	m_coreRunning(xdl::xdl_true),
	m_mouse_captured(xdl::xdl_false),
	m_drawMode(1),

	m_gBuffer(nullptr),
	m_camera(nullptr),
	m_frustum(nullptr),
	m_textureServer(nullptr),
	m_shadowMap(nullptr),
	m_gausBlur(nullptr),
	m_depthOfField(nullptr),
	m_font(nullptr),
	m_textEngine(nullptr),
	m_physics(nullptr),
	m_light(nullptr),
	m_skybox(nullptr),
	m_debugMode(xdl::xdl_false),
	m_spaceShip(nullptr),
	m_gameInputHandler(nullptr),
	m_selectedActor(nullptr),
	m_cameraMode(xdl::xdl_false),
	m_numberOfVertices(0),
	m_numberOfFaces(0),
	m_splashScreen(nullptr) {

	// Register Singularity as a listener to the event system.
	getCore()->registerListener(this);


}

Singularity::~Singularity() {

	//
	// Delete all vertex array objects.
	// TODO This method of delete will crash because some meshes are referenced. Change the method of owner ship.
	//
//	for(auto actorObject : m_actors) {
//		for(auto mesh : actorObject->getModel()->getMeshList()) {
//			get3DProcessor()->destroy(mesh->getVertexArray());
//		}
//	}
	delete m_camera;
	delete m_frustum;
	delete m_skybox;
	delete m_font;
	delete m_physics;
	delete m_light;
	delete m_shadowMap;
	delete m_depthOfField;
	delete m_gBuffer;
	delete m_gausBlur;
	delete m_fbVertexShader;
	delete m_fbFragmentShader;
	delete vb_framebufferArray;
}

xdl::XdevLOpenGLContext* Singularity::getOpenGLContext() {
	return m_openglContext;
}

xdl::XdevLOpenGL330*	Singularity::get3DProcessor() {
	return m_opengl;
}

const xdl::XdevLID& Singularity::getID() const {
	return m_id;
}


void Singularity::main(const Arguments& argv) throw() {

	//
	// Set title of the window.
	//
	std::wstring wtmp(L"The Singualrity - ");
	std::wstringstream wmajor, wminor, wpatch;
	wmajor << MAJOR;
	wminor << MINOR;
	wpatch << PATCH;
	m_version = L"Version: " + wmajor.str() + L"." + wminor.str() + L"." + wpatch.str();
	wtmp += m_version;

	std::string tmp2("The Singualrity - ");
	std::stringstream major, minor, patch;
	major << MAJOR;
	minor << MINOR;
	patch << PATCH;

	xdl::XdevLWindowTitle title {tmp2 + "Version: " + major.str() + "." + minor.str() + "." + patch.str()};

	getWindow()->setTitle(title);

	//m_splashScreen = new soan::utils::SplashScreen(getCore());
	//m_splashScreen->show();

	//
	// Initialize everything.
	//
	if(initializeRenderSystem() != xdl::ERR_OK) {
		return;
	}

	if(initializeEngine() != xdl::ERR_OK) {
		return;
	}

	if(initializeInputConnections() != xdl::ERR_OK) {
		return;
	}

	if(initializeEngine() != xdl::ERR_OK) {
		return;
	}

	if(initializeAssets() != xdl::ERR_OK) {
		return;
	}

//	m_splashScreen->hide();


	getWindow()->show();

	//
	// Start main loop.
	//
	m_start_time 	= getCore()->getTime();
	m_old_time		= m_start_time;

	// Start main loop.
	while(!esc->getClicked() && (m_coreRunning == true)) {

		// Update the Core System.
		getCore()->update();

		m_dT 				= getCore()->getTime() - m_old_time;
		m_old_time 	= getCore()->getTime();

		// Do the physics.
		handleInputEvents(m_dT);

		handleActors(m_dT);

		handlePhysics(m_dT);

		if(m_cameraMode == xdl::xdl_false) {
			m_camera->setTrackingProperties(0.0f, -10.0f, 100.0f, m_dT);
		}

		m_frustum->update(m_camera, true);

		tmath::matrix<float, 4, 6>& frustum = m_frustum->getFrustumMatrix();
		btVector3 from, to1, to2, to3;

		from.setX(m_frustum->getPoints(0).x);
		from.setY(m_frustum->getPoints(0).y);
		from.setZ(m_frustum->getPoints(0).z);

		to1.setX(m_frustum->getPoints(1).x);
		to1.setY(m_frustum->getPoints(1).y);
		to1.setZ(m_frustum->getPoints(1).z);

		to2.setX(m_frustum->getPoints(2).x);
		to2.setY(m_frustum->getPoints(2).y);
		to2.setZ(m_frustum->getPoints(2).z);

		to3.setX(m_frustum->getPoints(3).x);
		to3.setY(m_frustum->getPoints(3).y);
		to3.setZ(m_frustum->getPoints(3).z);

		m_debugRenderer->drawLine(from, to1, btVector3(1,1,1),  btVector3(1,1,1));
		m_debugRenderer->drawLine(from, to2, btVector3(1,1,1),  btVector3(1,1,1));
		m_debugRenderer->drawLine(from, to3, btVector3(1,1,1),  btVector3(1,1,1));

		for(auto renderable : m_renderable) {
			renderable->getCollisionShape();
			if(m_frustum->isPointInside(renderable->getPosition())) {
				std::cout << "Inside the Frustum: " << renderable->getPosition() << std::endl;
				renderable->setCastShadow(xdl::xdl_false);
			} else {
				renderable->setCastShadow(xdl::xdl_true);
			}
		}

		handleGraphics(m_dT);

	}

}



void  Singularity::handleGraphics(double dT) {
//	xdl::XdevLOpenGLContextScope scope(get3DProcessor(), getWindow());
	getOpenGLContext()->makeCurrent(getWindow());

	calculateShadowMaps();


	startDeferredLighting();


	m_gausBlur->setInputTexture(0, m_gBuffer->getTexture(soan::GBuffer::LIGHTING));
	m_gausBlur->setBlurSize(1.0, 1.0);
	m_gausBlur->apply();

	//
	// Draw final framebuffer
	//
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, getWindow()->getWidth(), getWindow()->getHeight());
	tmath::mat4 fbProjection;
	tmath::ortho(0.0f,
	             (float)getWindow()->getWidth(),
	             0.0f,
	             (float)getWindow()->getHeight(),
	             -1.0f,
	             1.0f, fbProjection);
	m_fbShaderProgram->activate();
	m_fbShaderProgram->setUniformMatrix4(testProj, 1, fbProjection);
	m_fbShaderProgram->setUniformi(testTex, 0);

	switch(m_drawMode) {
		case 0:
			m_gausBlur->getOutputTexture(0)->activate(0);
			break;
		case 1:
			m_gBuffer->getTexture(soan::GBuffer::LIGHTING)->activate(0);
			break;
		case 2:
			m_gBuffer->getTexture(soan::GBuffer::DIFFUSE)->activate(0);
			break;
		case 3:
			m_gBuffer->getTexture(soan::GBuffer::POSITION)->activate(0);
			break;
		case 4:
			m_gBuffer->getTexture(soan::GBuffer::NORMAL)->activate(0);
			break;
		case 5:
			m_shadowMap->getOutputTexture(0)->activate(0);
//			m_gBuffer->getTexture(soan::GBuffer::DEPTH)->activate(0);
			break;
		case 6:
			m_depthOfField->getOutputTexture(0)->activate(0);
			break;
	}
	m_fbShaderProgram->deactivate();

	m_opengl->setActiveVertexArray(vb_framebufferArray);
	m_opengl->setActiveShaderProgram(m_fbShaderProgram);

	m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);






	m_fpsCounter.update(m_dT);
	if(getCore()->getTime() - m_start_time > 1.0) {
		std::cout << m_fpsCounter.getMeanFPS() << std::endl;
		m_start_time = getCore()->getTime();

		ss.str(L"");
		ss.clear();

		ss << m_fpsCounter.getMeanFPS();
	}


	// Do the font rendering.
	//
//	glClearColor(1.0, 0.0, 0.0, 1.0);
//	glClear(GL_COLOR_BUFFER_BIT);


	//
	// Set the version onto the screen.
	//
	std::wstring message = m_version;
	m_textEngine->setColor(255, 255, 255, 255);
	m_textEngine->setDFT(0);
	m_textEngine->setEffect(0);
	m_textEngine->setScale(1.0f);
	m_textEngine->addDynamicText(message, -1.0, 0.96);

	//
	// Set the FPS onto the screen.
	//
	std::wstring tmp = L"FPS: " + ss.str();
	m_textEngine->addDynamicText(tmp.c_str(), -1.0, -0.98);


	std::wstringstream nov, nof;
	nov << m_numberOfVertices;
	nof << m_numberOfFaces;
	std::wstring maxVertices = L"Max Vertices: " + nov.str() + L"\nMax Faces   : " + nof.str();
	m_textEngine->addDynamicText(maxVertices.c_str(), -0.30, -0.92);

	m_textEngine->render();

	getOpenGLContext()->swapBuffers();
}





xdl::xdl_int Singularity::notify(xdl::XdevLEvent& event) {
	switch(event.type) {
		case xdl::XDEVL_CORE_EVENT: {
			if(event.core.type == xdl::XDEVL_CORE_SHUTDOWN) {
				std::cout << "XDEVL_CORE_SHUTDOWN received.\n";
				m_coreRunning = xdl::xdl_false;
			}
		}
		break;
		case xdl::XDEVL_MOUSE_MOTION: {
			m_mouse_x = event.motion.x;
			m_mouse_y = event.motion.y;
		}
		break;
	}
	return xdl::ERR_OK;
}

soan::TextureServer* Singularity::getTextureServer() {
	return m_textureServer;
}

xdl::xdl_int Singularity::initializeRenderSystem() {
	// Get the OpenGL context.
	m_openglContext = xdl::getModule<xdl::XdevLOpenGLContext*>(getCore(), xdl::XdevLID("MyOpenGLContext"));
	if(!m_openglContext) {
		return xdl::ERR_ERROR;
	}
	
	if(m_openglContext->create(getWindow()) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Get the OpenGL Rendering System.
	m_opengl = xdl::getModule<xdl::XdevLOpenGL330*>(getCore(), xdl::XdevLID("MyOpenGL"));
	if(!m_opengl) {
		return xdl::ERR_ERROR;
	}

	// We must attach the OpenGL context to a render m_window.
	if(m_opengl->create(getWindow()) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Get the FontSystem
	m_fontSystem = xdl::getModule<xdl::XdevLFontSystem*>(getCore(), xdl::XdevLID("MyFontSystem"));
	if(!m_fontSystem) {
		return xdl::ERR_ERROR;
	}

	// Get the Text Layout System.
	m_textEngine = xdl::getModule<xdl::XdevLTextLayout*>(getCore(), xdl::XdevLID("MyTextLayout"));
	if(!m_textEngine) {
		return xdl::ERR_ERROR;
	}


//	glewExperimental= GL_TRUE;
//	GLenum err = glewInit();
//	if(GLEW_OK != err) {
//		XDEVL_MODULE_ERROR(glewGetErrorString(err) << std::endl);
//		return xdl::ERR_ERROR;
//	}

	return xdl::ERR_OK;
}

xdl::xdl_int Singularity::initializeEngine() {
	Singularity::m_textureServer = soan::TextureServer::Inst();
	soan::TextureServer::Inst()->init(get3DProcessor(), "resources/models/");

	m_debugRenderer = new soan::SingularityDebugDrawer(get3DProcessor());

	m_physics = new soan::phys::Physics();
	m_physics->init(m_debugRenderer);



	// Initialize font system.
	m_fontSystem->init(getWindow()->getWidth(), getWindow()->getHeight(), get3DProcessor());

	m_font = m_fontSystem->createFromFontFile("resources/fonts/default_info.txt");

	m_textEngine->init(getWindow(), get3DProcessor());
	m_textEngine->setScale(1.0f);
	m_textEngine->setDFT(0);
	m_textEngine->setEffect(0);
	m_textEngine->useFont(m_font);

	unsigned int w = getWindow()->getWidth();
	unsigned int h = getWindow()->getHeight();

//	w =	640;
//	h = 400;

	// Create gaus blur post process effect.
	m_gausBlur = new soan::GausBlur(get3DProcessor());
	m_gausBlur->init(w,h, xdl::XDEVL_FB_COLOR_RGBA);

	// Create shadow map processor.
	m_shadowMap = new soan::ShadowMap(get3DProcessor(), soan::ShadowMap::NORMAL);
	m_shadowMap->init(2048, 2048);

	// Create depth of field post process effect.
	m_depthOfField = new soan::DepthOfField(get3DProcessor());
	m_depthOfField->init(w,h);

	// Create one camera.
	m_camera = new soan::Camera();

	// Create the frustum class.
	m_frustum = new soan::Frustum();

	// Create the G-Buffer which does the deferred rendering.
	m_gBuffer = new soan::GBuffer(get3DProcessor());
	if(m_gBuffer->init(w,h) == xdl::ERR_ERROR) {
		return xdl::ERR_ERROR;
	}
	m_gBuffer->setCamera(m_camera);
	m_gBuffer->setShadowMap(m_shadowMap);

	m_camProjectionsMatrix 		= m_gBuffer->getFillGBufferShaderProgram()->getUniformLocation("projMatrix");
	m_camViewMatrix					 	= m_gBuffer->getFillGBufferShaderProgram()->getUniformLocation("viewMatrix");
	m_modelMatrix							= m_gBuffer->getFillGBufferShaderProgram()->getUniformLocation("modelMatrix");


	m_opengl->createShaderProgram(&m_fbShaderProgram);

	if(m_opengl->createVertexShader(&m_fbVertexShader) == -1) {
		std::cerr << "Could not compile vertex shader." << std::endl;
		return xdl::ERR_ERROR;
	}
	m_fbVertexShader->compileFromFile("resources/shaders/fb.vs");

	if(m_opengl->createFragmentShader(&m_fbFragmentShader) == -1) {
		std::cerr << "Could not compile fragment shader." << std::endl;
		return xdl::ERR_ERROR;
	}
	m_fbFragmentShader->compileFromFile("resources/shaders/fb.fs");

	m_fbShaderProgram->attach(m_fbVertexShader);
	m_fbShaderProgram->attach(m_fbFragmentShader);
	m_fbShaderProgram->link();

	testProj 							= m_fbShaderProgram->getUniformLocation("projMatrix");
	testTex								= m_fbShaderProgram->getUniformLocation("diffuseTex");

	xdl::xdl_float screen_vertex [] = {
		0.0f, 0.0f,
		(xdl::xdl_float)getWindow()->getWidth(), 0.0f,
		(xdl::xdl_float)getWindow()->getWidth(), (xdl::xdl_float)getWindow()->getHeight(),

		(xdl::xdl_float)getWindow()->getWidth(), (xdl::xdl_float)getWindow()->getHeight(),
		0.0f, (xdl::xdl_float)getWindow()->getHeight(),
		0.0f, 0.0f
	};
	xdl::xdl_float screen_uv [] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};

	xdl::XdevLVertexDeclaration* vd = new xdl::XdevLVertexDeclaration();
	vd->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, soan::VERTEX_POSITION);
	vd->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, soan::VERTEX_TEXTURE_COORD);

	std::vector<xdl::xdl_uint8*> list;
	list.push_back((xdl::xdl_uint8*)screen_vertex);
	list.push_back((xdl::xdl_uint8*)screen_uv);

//	xdl::XdevLVertexBuffer* streamBuffer1;
//	xdl::XdevLVertexBuffer* streamBuffer2;
//
//	get3DProcessor()->createVertexBuffer(&streamBuffer1);
//	get3DProcessor()->createVertexBuffer(&streamBuffer2);
//
//	streamBuffer1->init((xdl::xdl_uint8*)screen_vertex, 6);
//	streamBuffer2->init((xdl::xdl_uint8*)screen_uv, 6);


	get3DProcessor()->createVertexArray(&vb_framebufferArray);
	vb_framebufferArray->init(list.size(), list.data(), 6, vd);

//	vb_framebufferArray->init((xdl::xdl_uint8*)list.data(), list.size(), vd);

//	vb_framebufferArray->init();
//	vb_framebufferArray->activate();
//	vb_framebufferArray->setVertexStreamBuffer(soan::VERTEX_POSITION, 2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, streamBuffer1);
//	vb_framebufferArray->setVertexStreamBuffer(soan::VERTEX_TEXTURE_COORD, 2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, streamBuffer2);
//	vb_framebufferArray->deactivate();


//	m_font2D->setScale(0.75f);
//	std::string message = "The quick brown fox jumps over the lazy dog.";
//	message += "\nThe quick brown fox jumps over the lazy dog.";
//	message += "\nThe quick brown fox jumps over the lazy dog.";
//	message += "\nThe quick brown fox jumps over the lazy dog.";
//	message += "\nThe quick brown fox jumps over the lazy dog.";
//
//	m_font2D->addStaticText(message.c_str(), -1.0, 0.4);

	return xdl::ERR_OK;

}

xdl::xdl_int Singularity::initializeAssets() {
	soan::TextureServer::Inst()->setResourcePathPrefix("resources/models/");

	m_camera->setPosition(0.0f, -7.0f, 10.0f);

	// Assimp to Singularity importer.
	soan::utils::AssimpToModel assimpToModel(get3DProcessor());

	//
	// We need a light source. Otherwise we will not see anything.
	//
	m_light = new soan::Light(soan::Light::DIRECTIONAL);
	m_light->rotateLocalX(-35.0f);
	m_light->setPosition(0.0f, 0.0f, 0.0f);

	//
	// This light will be added to the G-Buffer engine.
	//
	m_gBuffer->addLight(m_light);



//		model.loadAsset("resources/models/HN48/Federation Interceptor HN48 flying.obj");
//		model.loadAsset("resources/models/scene.obj");
//		model.loadAsset("resources/models/sponza/sponza.obj");
//		model.loadAsset("resources/models/sphere.obj");



	std::shared_ptr<soan::Model> model(new soan::Model(m_opengl));
	if(assimpToModel.import("resources/models/box.obj", model) == xdl::ERR_OK) {
		for(unsigned int as = 0; as < 100; as++) {
			soan::game::Astroid* astroid 	= new soan::game::Astroid();
			astroid->setModel(std::shared_ptr<soan::Model>(model->refCopy()));
			astroid->setPhysics(m_physics, 0.1f);
			astroid->setLifeTime(0);
			m_renderable.push_back(astroid);

			m_numberOfVertices += model->getNumberOfVertices();
			m_numberOfFaces += model->getNumberOfFaces();
		}
	} else {
		std::cerr << "Singularity::initializeAssets: Could not load box mesh." << std::endl;
		return xdl::ERR_ERROR;
	}


	std::shared_ptr<soan::Model> spaceModel(new soan::Model(get3DProcessor()));
	if(assimpToModel.import("resources/models/HN48/FederationInterceptor1.obj", spaceModel) != xdl::ERR_OK) {
//	if(assimpToModel.import("resources/models/HN21/space_ship.obj", spaceModel) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	m_spaceShip = new soan::game::SpaceShip();
	m_spaceShip->setName("SpaceShip");
	m_spaceShip->setModel(spaceModel);
	m_spaceShip->setLifeTime(0);
	m_spaceShip->setPhysics(m_physics, 22.0f);

	m_renderable.push_back(m_spaceShip);

	m_numberOfVertices += spaceModel->getNumberOfVertices();
	m_numberOfFaces += spaceModel->getNumberOfFaces();

	m_selectedActor = m_spaceShip;
	m_camera->startTrackObject(m_spaceShip);



	std::shared_ptr<soan::Model> groundModel(new soan::Model(get3DProcessor()));
	if(assimpToModel.import("resources/models/plane.obj", groundModel) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}
	xdl::XdevLTexture* displacementMap = soan::TextureServer::Inst()->import("bricks_displace.jpg");
	displacementMap->lock();
	displacementMap->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
	displacementMap->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR);
	displacementMap->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_REPEAT);
	displacementMap->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_REPEAT);
	displacementMap->unlock();

	soan::game::Actor* ground = new soan::game::SpaceShip();
	ground->setName("Ground");
	ground->setLifeTime(0);
	ground->setModel(groundModel);
	ground->setPosition(0.0f, -10.0f, 0.0f);
	ground->setPhysics(m_physics, 0.0f);

	m_material = ground->getModel()->getMesh(0)->getMaterial();
	m_model			= groundModel;

	ground->getModel()->getMesh(0)->getMaterial()->setUseDiffuseConst(xdl::xdl_false);
	ground->getModel()->getMesh(0)->getMaterial()->setRoughness(0.1);
	ground->getModel()->getMesh(0)->getMaterial()->setTexture(soan::Material::DISPLACEMENT_MAP, displacementMap);
	ground->getModel()->getMesh(0)->getMaterial()->setUseDisplacementMap(xdl::xdl_true);
	ground->getModel()->getMesh(0)->getMaterial()->setUseNormalMap(xdl::xdl_false);
	m_numberOfVertices += groundModel->getNumberOfVertices();
	m_numberOfFaces += groundModel->getNumberOfFaces();
	m_renderable.push_back(ground);





	xdl::xdl_float rgb [] = {1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0};

	std::shared_ptr<soan::Model> planetModel(new soan::Model(get3DProcessor()));
	if(assimpToModel.import("resources/models/sphere.obj", planetModel) == xdl::ERR_OK) {
		for(unsigned int as = 0; as < 2; as++) {
			soan::game::Planet* planet = new soan::game::Planet();
			planet->setName("Earth");
			planet->setModel(std::shared_ptr<soan::Model>(planetModel->refCopy()));
			planet->setLifeTime(0);
			planet->getModel()->getMesh(0)->getMaterial()->setUseDiffuseConst(true);
			planet->getModel()->getMesh(0)->getMaterial()->setDiffuse(rgb[as*4 + 0], rgb[as*4 + 1], rgb[as*4 + 2], rgb[as+4 + 3]);
			planet->getModel()->getMesh(0)->getMaterial()->setRoughness(0.1);
			planet->setPhysics(m_physics, 10.0);
			planet->setPosition(0.0f, 0.0f, -100.0f);
			m_renderable.push_back(planet);

			m_numberOfVertices 	+= planetModel->getNumberOfVertices();
			m_numberOfFaces 		+= planetModel->getNumberOfFaces();

			//		m_selectedActor = planet;
			//	m_camera->startTrackObject(planet->getModel());
		}
	} else {
		return xdl::ERR_ERROR;
	}






	m_skybox = new soan::game::SkyBox(get3DProcessor());
	m_skybox->init();
	m_skybox->getMaterial().setUseDiffuseConst(xdl::xdl_false);
	m_skybox->getMaterial().setUseNormalMap(xdl::xdl_false);




	return xdl::ERR_OK;
}

xdl::xdl_int Singularity::initializeInputConnections() {

	m_gameInputHandler = new soan::game::SpaceShipInputHandler(getKeyboard(), getMouse(), getJoystick());
	m_gameInputHandler->init();

	//
	// Get all mouse references.
	//

	getMouse()->getButton(xdl::BUTTON_LEFT, &left_mouse_button);
	getMouse()->getButton(xdl::BUTTON_RIGHT, &right_mouse_button);
	getMouse()->getAxis(xdl::AXIS_X, &mouse_x);
	getMouse()->getAxis(xdl::AXIS_Y, &mouse_y);

	//
	// Get a valid button objects.
	//
	getKeyboard()->getButton(xdl::KEY_ESCAPE, &esc);
	getKeyboard()->getButton(xdl::KEY_A, 					&key_a);
	getKeyboard()->getButton(xdl::KEY_D, 					&key_d);
	getKeyboard()->getButton(xdl::KEY_W, 					&key_w);
	getKeyboard()->getButton(xdl::KEY_S, 					&key_s);
	getKeyboard()->getButton(xdl::KEY_Q, 					&key_q);
	getKeyboard()->getButton(xdl::KEY_E, 					&key_e);
	getKeyboard()->getButton(xdl::KEY_C, 					&key_c);
	getKeyboard()->getButton(xdl::KEY_SPACE, 			&key_space);
	getKeyboard()->getButton(xdl::KEY_G, 					&key_g);
	getKeyboard()->getButton(xdl::KEY_R, 					&key_r);
	getKeyboard()->getButton(xdl::KEY_M, 					&key_m);

	getKeyboard()->getButton(xdl::KEY_1, 					&key_1);
	getKeyboard()->getButton(xdl::KEY_2, 					&key_2);
	getKeyboard()->getButton(xdl::KEY_3, 					&key_3);
	getKeyboard()->getButton(xdl::KEY_4, 					&key_4);
	getKeyboard()->getButton(xdl::KEY_5, 					&key_5);
	getKeyboard()->getButton(xdl::KEY_6, 					&key_6);
	getKeyboard()->getButton(xdl::KEY_7, 					&key_7);
	getKeyboard()->getButton(xdl::KEY_8, 					&key_8);
	getKeyboard()->getButton(xdl::KEY_9, 					&key_9);
	getKeyboard()->getButton(xdl::KEY_0, 					&key_0);
	getKeyboard()->getButton(xdl::KEY_BACKSLASH, 	&key_backslash);
	getKeyboard()->getButton(xdl::KEY_SLASH, 			&key_slash);

	getKeyboard()->getButton(xdl::KEY_COMMA, 			&key_comma);

	getKeyboard()->getButton(xdl::KEY_KP_PLUS, 			&key_kp_plus);
	getKeyboard()->getButton(xdl::KEY_KP_MINUS,			&key_kp_minus);



	return xdl::ERR_OK;
}

void Singularity::handleInputEvents(double dT) {

	m_gameInputHandler->handleInput(m_spaceShip, dT);

	//
	// Do all input event handling here.
	//
	if(key_w->getPressed()) {
		m_camera->moveForward(dT);
	}
	if(key_s->getPressed()) {
		m_camera->moveForward(-dT);
	}
	if(key_a->getPressed()) {
		m_camera->moveSide(-dT);
	}
	if(key_d->getPressed()) {
		m_camera->moveSide(dT);
	}
	if(key_c->getPressed()) {
		m_camera->moveUp(-dT);
	}
	if(key_space->getPressed()) {
		m_camera->moveUp(dT);
	}
	if(key_q->getPressed()) {
		m_camera->doRoll(-50.0f*dT);
	}
	if(key_e->getPressed()) {
		m_camera->doRoll(50.0f*dT);
	}
	if(key_1->getClicked()) {
		m_drawMode = 0;
	}
	if(key_2->getClicked()) {
		m_drawMode = 1;
	}
	if(key_3->getClicked()) {
		m_drawMode = 2;
	}
	if(key_4->getClicked()) {
		m_drawMode = 3;
	}
	if(key_5->getClicked()) {
		m_drawMode = 4;
	}
	if(key_6->getClicked()) {
		m_drawMode = 5;
	}
	if(key_7->getClicked()) {
		m_drawMode = 6;
	}
	if(key_slash->getClicked()) {
		m_debugMode = !m_debugMode;
	}

	if(left_mouse_button->getClicked()) {
		m_mouse_captured = !m_mouse_captured;

		if(m_mouse_captured == xdl::xdl_true) {
			getWindow()->hidePointer();
			getMouse()->setRelativeMode(xdl::xdl_true);
		} else {
			getWindow()->showPointer();
			getMouse()->setRelativeMode(xdl::xdl_false);
		}
	}

	if(m_mouse_captured) {

		static xdl::xdl_float dy = 0.0f;
		static xdl::xdl_float dx = 0.0f;


		dy = -mouse_y->getDeltaValue()*dT*0.2;
		dx = -mouse_x->getDeltaValue()*dT*0.2;

		m_camera->fpsView(dy, dx, dT);


	}

	if(right_mouse_button->getClicked()) {
		soan::game::Bullet* bullet = new soan::game::Bullet();
		m_renderable.push_back(bullet);
		std::cout << "Actor: " << bullet->getName() << " created.\n";
	}

	if(key_r->getClicked()) {
		m_spaceShip->getRigidBody()->clearForces();
		m_spaceShip->reset();

	}

	if(key_m->getClicked()) {
		m_cameraMode = !m_cameraMode;

		if(!m_cameraMode) {
			m_camera->startTrackObject(m_spaceShip);
		} else {
			m_camera->stopTrackObject();
		}
		m_gameInputHandler->redirect(m_cameraMode);
	}



	static xdl::xdl_float rd = 0.00002;
	if(key_kp_plus->getClicked()) {
		rd += 0.00001;
		m_shadowMap->setMinVariance(rd);
	}
	if(key_kp_minus->getClicked()) {
		rd -= 0.00001;
		m_shadowMap->setMinVariance(rd);
	}

	static xdl::xdl_bool ms = xdl::xdl_true;
	if(key_0->getClicked()) {
		ms = !ms;
		m_material->setUseDisplacementMap(ms);
	}

	static xdl::xdl_bool ns = xdl::xdl_true;
	if(key_9->getClicked()) {
		ns = !ns;
		m_material->setUseNormalMap(ns);
	}

}

void Singularity::handlePhysics(double dT) {
	m_physics->stepSimulation(dT);
}

void Singularity::handleActors(double dT) {

	for(auto actor = m_renderable.begin(); actor != m_renderable.end(); ++actor) {
		if((*actor)->update(dT*1000) == xdl::xdl_true) {
			delete(*actor);
			actor = m_renderable.erase(actor);
		} else {
			// Do something else, for example draw it.

		}
	}

}

void Singularity::calculateShadowMaps() {
	//
	// Do shadow mapping.
	//
	m_shadowMap->setCamera(m_camera);
	m_shadowMap->setLight(m_light);
	m_shadowMap->activate();

	get3DProcessor()->setActiveShaderProgram(m_shadowMap->getShaderProgram());

	for(auto& actorObject : m_renderable) {
		if(!actorObject->isRenderingEnabled() || !actorObject->getCastShadow()) {
			continue;
		}
		m_shadowMap->setModelMatrix(m_light->getInverseTransformationMatrix());


		for(auto& mesh : actorObject->getModel()->getMeshList()) {

			m_shadowMap->setModelMatrix(actorObject->getTransformationMatrix());

			get3DProcessor()->setActiveVertexArray(mesh->getVertexArray());

			get3DProcessor()->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, mesh->getNumberOfFaces() * 3);
		}
	}

	m_shadowMap->deactivate();

}

void Singularity::startDeferredLighting() {


	//m_light->setPosition(m_camera->getPosition());

//	glEnable(GL_MULTISAMPLE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//
	// Do Deferred Shading
	//
	m_gBuffer->startFillGBuffer();

	//
	// Set neccessary information for the shader program.
	//
	m_gBuffer->setProjectionMatrix(m_camera->getProjectionMatrix());

	//
	// Set camera values to the shader program.
	//

	m_gBuffer->setDepthBias(m_shadowMap->getDepthBias());


	get3DProcessor()->setActiveShaderProgram(m_gBuffer->getFillGBufferShaderProgram());


	//
	// Render Skybox.
	//


	tmath::mat4 identity, orientation;
	tmath::identity(identity);
	tmath::convert(m_camera->getInverseOrientation(), orientation);
	m_gBuffer->setViewMatrix(orientation);
	m_gBuffer->setModelMatrix(identity);

	if(m_skybox != nullptr) {
		m_gBuffer->setReflectionTextureCube(m_skybox->getSkyBoxTexture(), 1);
		m_gBuffer->getGBuffer()->activateDepthTarget(xdl::xdl_false);
		m_gBuffer->setMaterial(m_skybox->getMaterial());

		m_skybox->render();

		m_gBuffer->setReflectionTextureCube(m_skybox->getSkyBoxTexture(), 0);
		m_gBuffer->getGBuffer()->activateDepthTarget(xdl::xdl_true);
	} else {
		m_gBuffer->setReflectionTextureCube(nullptr, 0);
	}



	//
	// Full transformation of the camera.
	//
	m_gBuffer->setViewMatrix(m_camera->getInverseTransformationMatrix());

	//
	// Render all actors.
	//

	for(auto& actorObject : m_renderable) {
		if(!actorObject->isRenderingEnabled()) {
			continue;
		}

		for(auto& mesh : actorObject->getModel()->getMeshList()) {
			m_gBuffer->setMaterial(*mesh->getMaterial());
			m_gBuffer->setModelMatrix(actorObject->getTransformationMatrix());

			get3DProcessor()->setActiveVertexArray(mesh->getVertexArray());
			get3DProcessor()->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, mesh->getNumberOfFaces() * 3);

		}

	}


	if(xdl::xdl_true == m_debugMode) {



		for(auto& actorObject : m_renderable) {
			if(!actorObject->isRenderingEnabled()) {
				continue;
			}

			//
			// Forward vector -z
			//
			tmath::vec3 tmp = actorObject->getPosition();
			btVector3 from(tmp.x, tmp.y, tmp.z);

			tmath::quat tmpQuat = actorObject->getOrientation();
			btQuaternion orientation(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);

			btQuaternion forwardq = orientation * btVector3(0.0f, 0.0f, 1.0f);
			forwardq *= 5.0f;
			btVector3 forward(from.x() + forwardq.x(), from.y() + forwardq.y(), from.z() + forwardq.z());
			btVector3 forwardColor(0.0f, 0.0f, 1.0f);
			m_debugRenderer->drawLine(from, forward, forwardColor, forwardColor);

			//
			// Right vector x
			//
			btQuaternion rightq = orientation * btVector3(1.0f, 0.0f, 0.0f);
			rightq *= 5.0f;
			btVector3 right(from.x() + rightq.x(), from.y() + rightq.y(), from.z() + rightq.z());
			btVector3 rightColor(1.0f, 0.0f, 0.0f);
			m_debugRenderer->drawLine(from, right, rightColor, rightColor);

			//
			// Up vector y
			//
			btQuaternion upq = orientation * btVector3(0.0f, 1.0f, 0.0f);
			upq *= 5.0f;
			btVector3 up(from.x() + upq.x(), from.y() + upq.y(), from.z() + upq.z());
			btVector3 upColor(0.0f, 1.0f, 0.0f);
			m_debugRenderer->drawLine(from, up, upColor, upColor);

		}

		//
		// We have set the model matrix to identity because the lines drawn by bullet are
		// transformed already.
		//
		tmath::mat4 identity;
		tmath::identity(identity);

		m_gBuffer->setModelMatrix(identity);
		m_gBuffer->setDebugMode(1);
		m_physics->debugDrawWorld(m_gBuffer->getProjectionMatrix());
		m_gBuffer->setDebugMode(0);
	}


	m_gBuffer->stopFillGBuffer();



	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	m_gBuffer->startLightingStage();


	if(m_drawMode == 6) {
		m_depthOfField->setInputTexture(0, m_gBuffer->getTexture(soan::GBuffer::LIGHTING));
		m_depthOfField->setInputTexture(1, m_gBuffer->getTexture(soan::GBuffer::DEPTH));
		m_depthOfField->apply();
	}

}

XdevLStartMain(Singularity, "resources/ini/singularity.xml")
