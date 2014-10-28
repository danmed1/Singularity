#ifndef SINGULARITY_H
#define SINGULARITY_H

#include <XdevL.h>
#include <XdevLApplication.h>
#include <XdevLOpenGL/XdevLOpenGL.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLRingBuffer.h>
#include <cmath>
#include <vector>
#include <list>
#include <tm/tm.h>


#include "Engine/Camera.h"
#include "Engine/Frustum.h"
#include "Engine/Moveable.h"
#include "Engine/Entity/Model.h"
#include "Engine/GBuffer.h"
#include "Engine/TextureServer.h"
#include "Engine/Effects/GausBlur.h"
#include "Engine/Effects/DepthOfField.h"
#include "Engine/Effects/ToneMapping.h"
#include "Engine/Effects/ShadowMap.h"
#include "Engine/Light.h"
#include "Engine/Physics/Physics.h"
#include "Engine/SkyBox.h"

#include "Game/SpaceShipInputHandler.h"
#include "Game/Universe/Astroid.h"
#include "Game/Universe/Planet.h"
#include "Game/Universe/Star.h"
#include "Game/Bullet.h"
#include "Game/SpaceShip.h"

#include "Utils/SplashScreen.h"
#include "Utils/FPSCounter.h"
#include "Engine/Fonts/XdevLFontImpl.h"
#include "Engine/Fonts/XdevLTextLayoutImpl.h"


const xdl::xdl_uint MAJOR = 0;
const xdl::xdl_uint MINOR = 4;
const xdl::xdl_uint PATCH = 2;

/**
	@class Singularity
	@brief The main class.
*/
class Singularity : public xdl::XdevLApplication {
	public:

		Singularity(int argc, char** argv, const char* xml_filename) throw();

		virtual ~Singularity();

		/// The main starting point for Singularity.
		virtual void main(const Arguments& argv) throw() override;

		/// Returns the texture server.
		soan::TextureServer* getTextureServer();

		/// Initialize the render system.
		xdl::xdl_int initializeRenderSystem();

		/// Initialize the gfx engine.
		xdl::xdl_int initializeEngine();

		/// Initialize all input connections.
		xdl::xdl_int initializeInputConnections();

		/// Initialize all assets.
		xdl::xdl_int initializeAssets();

		/// Handle for occuring events from XdevL messaging system.
		virtual xdl::xdl_int notify(xdl::XdevLEvent& event) override;

		/// Handle for input events.
		void handleInputEvents(double dT);

		/// Handle for the physics simulation.
		void handlePhysics(double dT);

		/// Handle for all actors.
		void handleActors(double dT);

		/// Handle for all gfx stuff.
		void handleGraphics(double dT);

		/// Returns the ID for message handling.
		virtual xdl::XdevLID* getID() final;

	private:

		/// Returns the 3D processor, which is at the moment OpenGL :D.
		xdl::XdevLOpenGL330*					get3DProcessor();

		/// Calculates the shadow map for the scene.
		void calculateShadowMaps();

		/// Starts the deferred lighting process.
		void startDeferredLighting();

	private:
		xdl::XdevLID									m_id;
		xdl::XdevLOpenGL330* 					m_opengl;
		xdl::XdevLVertexArray* 				vb_framebufferArray;
		xdl::XdevLVertexShader*				m_fbVertexShader;
		xdl::XdevLFragmentShader*		m_fbFragmentShader;
		xdl::XdevLShaderProgram* 		m_fbShaderProgram;

		xdl::xdl_int									m_camProjectionsMatrix;
		xdl::xdl_int									m_camViewMatrix;
		xdl::xdl_int									m_modelMatrix;


		xdl::xdl_bool 								m_coreRunning;
		xdl::xdl_uint									m_mouse_x;
		xdl::xdl_uint									m_mouse_y;

		xdl::xdl_int									testProj;
		xdl::xdl_int 									testTex;



		xdl::xdl_bool 								m_mouse_captured;
		xdl::xdl_uint									m_drawMode;

		soan::GBuffer* 								m_gBuffer;
		soan::Camera*									m_camera;
		soan::Frustum*								m_frustum;
		soan::TextureServer* 					m_textureServer;
		soan::ShadowMap*							m_shadowMap;
		soan::GausBlur* 							m_gausBlur;
		soan::DepthOfField* 					m_depthOfField;
		soan::XdevLFontImpl*					m_font2D;
		soan::XdevLTextLayout*				m_textEngine;
		soan::phys::Physics*					m_physics;
		soan::Light*									m_light;
		soan::game::SkyBox*						m_skybox;

		soan::utils::FPSCounter 			m_fpsCounter;


		std::list<soan::game::Renderable*> m_renderable;


		xdl::XdevLButton* left_mouse_button;
		xdl::XdevLButton* right_mouse_button;

		xdl::XdevLAxis* 	mouse_x;
		xdl::XdevLAxis* 	mouse_y;

		xdl::XdevLButton* esc;
		xdl::XdevLButton* key_a;
		xdl::XdevLButton* key_b;
		xdl::XdevLButton* key_c;
		xdl::XdevLButton* key_d;
		xdl::XdevLButton* key_e;
		xdl::XdevLButton* key_f;
		xdl::XdevLButton* key_g;
		xdl::XdevLButton* key_h;
		xdl::XdevLButton* key_i;
		xdl::XdevLButton* key_j;
		xdl::XdevLButton* key_k;
		xdl::XdevLButton* key_l;
		xdl::XdevLButton* key_m;
		xdl::XdevLButton* key_n;
		xdl::XdevLButton* key_o;
		xdl::XdevLButton* key_p;
		xdl::XdevLButton* key_q;
		xdl::XdevLButton* key_r;
		xdl::XdevLButton* key_s;
		xdl::XdevLButton* key_t;
		xdl::XdevLButton* key_u;
		xdl::XdevLButton* key_v;
		xdl::XdevLButton* key_w;
		xdl::XdevLButton* key_x;
		xdl::XdevLButton* key_y;
		xdl::XdevLButton* key_z;
		xdl::XdevLButton* key_space;
		xdl::XdevLButton* key_1;
		xdl::XdevLButton* key_2;
		xdl::XdevLButton* key_3;
		xdl::XdevLButton* key_4;
		xdl::XdevLButton* key_5;
		xdl::XdevLButton* key_6;
		xdl::XdevLButton* key_7;
		xdl::XdevLButton* key_8;
		xdl::XdevLButton* key_9;
		xdl::XdevLButton* key_0;
		
		xdl::XdevLButton*	key_backslash;
		xdl::XdevLButton*	key_slash;
		xdl::XdevLButton*	key_comma;
		xdl::XdevLButton*	key_colon;
		xdl::XdevLButton*	key_kp_plus;
		xdl::XdevLButton*	key_kp_minus;




		xdl::xdl_double m_start_time;
		xdl::xdl_double m_old_time;
		xdl::xdl_double m_dT;

		soan::SingularityDebugDrawer*			m_debugRenderer;
		xdl::xdl_bool												m_debugMode;

		soan::game::SpaceShip*							m_spaceShip;
		soan::game::SpaceShipInputHandler* m_gameInputHandler;
		soan::game::Actor*									m_selectedActor;
		
		xdl::xdl_bool 											m_cameraMode;

		
		std::wstring												m_version;
		xdl::xdl_uint												m_numberOfVertices;
		xdl::xdl_uint												m_numberOfFaces;		
		std::wstringstream 									ss;
		
		std::shared_ptr<soan::Material>	m_material;
		std::shared_ptr<soan::Model> 		m_model;
		
		soan::utils::SplashScreen*					m_splashScreen;
};



#endif
