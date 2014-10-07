#ifndef CAMERA_H
#define CAMERA_H

#include <tm/tm.h>
#include <tm/tmg.h>

#include "Moveable.h"
#include <memory>

namespace soan {


	/**
		@class Camera
		@brief A camera in the virtual space..
		
		The camera supports 2 modes at the moment.
		
		-# FPS mode     : You can move the camera using heading and pitch.
		-# Tracking mode: Camera will track a object at a specific distance.
		
	*/
	class Camera : public Moveable {
	public:
			
			/**
				@enum CameraProjectionType
				@brief Projections type of the camera.
			*/
			enum CameraProjectionType {
			  ORTHO,				/// Orhtographic projection.
			  PERSPECTIVE		/// Perspective projection.
			};

			Camera();
			
			virtual ~Camera();
			
			/// Rotate camera like a First Person game.
			/**
				The angles must be in world coordinate system.
			*/
			void fpsView(float pitch, float yaw, float dT = 1.0f);
			
			/// Look at a specific point with a specified up vector.
			void lookAt(tmath::vec3 target, tmath::vec3 up);
			
			/// Move camera forward/backward.
			/**
				@param value Amount to move in forward direction.
			*/
			void moveForward(float value);
			
			/// Move camera left/right.
			/**
				@param value Amount to move in forward sidways.
			*/
			void moveSide(float value);
			
			/// Move camera up/down.
			/**
				@param value Amount to move in upward direction.
			*/
			void moveUp(float value);
			
			/// Roll camera.
			/**
				@param value Amount to do a roll rotation in degrees.
			*/			
			void doRoll(float value);
			
			/// Pitch camera.
			/**
				@param value Amount to do a pitch rotation in degrees.
			*/	
			void doPitch(float value);
			
			/// Yaw camera.
			/**
				@param value Amount to do a yaw rotation in degrees.
			*/				
			void doYaw(float value);
					
			/// Return the cameras projections matrix.
			tmath::mat4& getProjectionMatrix();
			
			/// Returns the viewports width.
			unsigned int getViewPortWidth();
			
			/// Returns the viewports height.
			unsigned int getViewPortHeight();
			
			/// Sets the viewports dimensions.
			void setViewPort(unsigned int width, unsigned int height);
			
			/// Sets the projections type.
			void setProjection(CameraProjectionType type);
			
			/// Start to track object.
			void startTrackObject(const std::shared_ptr<Moveable>& moveable);
			
			/// Stop tracking object.
			void stopTrackObject();
			
			/// Set tracking object properties.
			void setTrackingProperties(float heading, float pitch, float distance);
			
			/// Returns if the tracking mode is active or not.
			bool isTrackingModeActive();
			
			/// Sets the forward speed.
			void setForwardSpeed(float forwardSpeed);
			
			/// Sets the side speed.
			void setSideSpeed(float sideSpeed);
			
			/// Sets the up speed.
			void setUpSpeed(float upSpeed);
			
			/// Sets the LERP speed
			void setSLERPSpeed(float slerp);
			
	private:
		
			// Holds the type of projecttion.
			CameraProjectionType 	m_type;
			
			// Holds the projection matrix.
			tmath::mat4 					m_projection;
			
			// Holds the cameras viewport width.
			unsigned int 					m_viewPortWidth;
			
			// Holds the cameras viewport height.
			unsigned int 					m_viewPortHeight;
			
			// Holds the tracked object.
			std::shared_ptr<Moveable>			m_trackedObject;
			
			float 								m_pitch;
			float									m_heading;
			float									m_roll;
			
			float									m_forwardSpeed;
			float									m_sideSpeed;
			float									m_upSpeed;
			
			float									m_slerpSpeed;
	};

}

#endif
