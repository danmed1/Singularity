#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Engine/Physics/Physics.h"
#include "Engine/Model.h"
#include <btBulletDynamicsCommon.h>
#include <memory>

namespace soan {

	namespace game {

		class Renderable {
			public:
				Renderable();
				
				Renderable(phys::Physics* physics, Model* model);

				virtual ~Renderable();

				xdl::xdl_int init() ;

				/// Returns the assigned model
				const std::shared_ptr<Model>& getModel();

				/// Sets the model.
				void setModel(const std::shared_ptr<Model>& model, xdl::xdl_uint collisionShape = 0);
				
				/// Return the rigid body.
				btRigidBody*	getRigidBody();
				
				virtual bool update(xdl::xdl_uint64 timeStep);
				
				xdl::xdl_bool isRenderingEnabled();
				
				xdl::xdl_bool getCastShadow();
				
				void setRenderingEnabled(xdl::xdl_bool state);
				
				void setCastShadow(xdl::xdl_bool state);
				
				void setPhysics(phys::Physics* physics, xdl::xdl_float mass);
				
				void reset();
			private:
				phys::Physics*						m_physics;
				std::shared_ptr<Model>		m_model;
				btRigidBody* 							m_body;
				btDefaultMotionState* 		m_motionState;
				btBoxShape*								m_colShape;	
				xdl::xdl_bool							m_renderingEnabled;
				xdl::xdl_bool							m_castShadow;
				xdl::xdl_float						m_mass;
		};

	}

}

#endif
