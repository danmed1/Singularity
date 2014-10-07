#ifndef MOVEABLE_H
#define MOVEABLE_H

#include <tm/tm.h>
#include <tm/tmg.h>


namespace soan {
	/**
		@class Moveable
		@brief A moveable object in 3D space.
	*/
	class Moveable {
		public:

			Moveable();

			/// Returns the forward direction of the object.
			tmath::vec3 getDirection();

			/// Return the position in world space.
			tmath::vec3& getPosition();

			/// Return the scale in world space.
			tmath::vec3& getScale();

			/// Return the orientation in world space.
			tmath::quat& getOrientation();

			/// Returns the inverse of the orientation.
			tmath::quat getInverseOrientation();

			/// Return the orientation in world space as matrix.
			tmath::mat4 getOrientationMatrix();

			/// Returns the position of the forward vector in world space in unit length.
			tmath::vec3 getForwardVector();
			
			/// Returns the position of the right vector in world space in unit length.
			tmath::vec3 getRightVector();

			/// Returns the position of the up vector in world space in unit length.
			tmath::vec3 getUpVector();

			/// Return the transformation in world space as a matrix.
			/**
				The transformation is calculated by Transformation = TranslationMatrix * OrientationMatrix * ScaleMatrix
				@return The transformation matrix.
			*/
			virtual tmath::mat4 getTransformationMatrix();

			/// Return the cameras inverse transformation matrix.
			/**
				@return The inverse matrix of the transformation matrix.
			*/
			virtual tmath::mat4 getInverseTransformationMatrix();

			/// Set the position of the object.
			void setPosition(float x, float y, float z);

			/// Set the position of the object.
			void setPosition(const tmath::vec3& position);

			/// Set the scale factor of the object.
			void setScale(float sx, float sy, float sz);

			/// Set the scale factor of the object.
			void setScale(const tmath::vec3& scale);

			/// Set the orientation.
			void setOrientation(float x, float y, float z, float w);

			/// Set the orientation of the object.
			void setOrientation(const tmath::quat& orientation);

			/// Move the object relative from the current position.
			void moveRelative(float x, float y, float z);

			/// Move the object relative from the current position.
			void moveRelative(const tmath::vec3& distance);

			/// Rotate around it's local X axis
			void rotateLocalX(float angle);

			/// Rotate around it's local Y axis
			void rotateLocalY(float angle);

			/// Rotate around it's local Z axis
			void rotateLocalZ(float angle);

		protected:

			// Holds the position of the object in world coordinate.
			tmath::vec3		m_position;

			// Holds the orientation of the object in world coordinate.
			tmath::quat		m_orientation;

			// Holds the scale factor of the object in world coorindate.
			tmath::vec3		m_scale;

	};

}

#endif
