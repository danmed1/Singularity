#include "SpaceShipInputHandler.h"
#include "Utils/TinyMathBullet3Utils.h"

namespace soan {
	namespace game {
		SpaceShipInputHandler::SpaceShipInputHandler(xdl::XdevLKeyboardDevice* kd,
		    xdl::XdevLMouseDevice* md,
		    xdl::XdevLJoystickDevice* jd) : 	m_kd(kd),
			m_md(md),
			m_jd(jd),
			m_initialized(xdl::xdl_false) {
		}

		SpaceShipInputHandler::~SpaceShipInputHandler() {

			//
			// Delete everything.
			//
			delete m_thrustCommand;
			delete m_thrustBackwardCommand;
			delete m_rollLeftCommand;
			delete m_rollRightCommand;
			delete m_pitchForwardCommand;
			delete m_pitchBackwardCommand;
			delete m_headLeftCommand;
			delete m_headRightCommand;
			delete m_thrustUpCommand;
			delete m_thrustDownCommand;
			delete m_thrustRightCommand;
			delete m_thrustLeftCommand;

			m_initialized = xdl::xdl_false;
		}

		xdl::xdl_int SpaceShipInputHandler::init() {
			assert(!m_initialized && "GameInputHandler::handleInput: Not initialized.");

			//
			// Connect key events to button objects.
			//
			m_kd->getButton(xdl::KEY_UP, 				&m_pitchUp);
			m_kd->getButton(xdl::KEY_DOWN, 			&m_pitchDown);
			m_kd->getButton(xdl::KEY_A,	 				&m_rollLeft);
			m_kd->getButton(xdl::KEY_D,					&m_rollRight);
			m_kd->getButton(xdl::KEY_LEFT, 			&m_headLeft);
			m_kd->getButton(xdl::KEY_RIGHT,			&m_headRight);
			m_kd->getButton(xdl::KEY_W,					&m_thrust);
			m_kd->getButton(xdl::KEY_S,					&m_thrustBackward);
			m_kd->getButton(xdl::KEY_SPACE,			&m_thrustUp);
			m_kd->getButton(xdl::KEY_C,					&m_thrustDown);
			m_kd->getButton(xdl::KEY_Q,					&m_thrustLeft);
			m_kd->getButton(xdl::KEY_E,					&m_thrustRight);

			
			m_jd->getAxis(xdl::JOY_AXIS_3,			&m_thrustAxis);
			m_thrustAxis->setMinMax(0.0f, 1.0f);
			
			//
			// Create all used commands.
			//
			m_thrustCommand						= new ThrustCommand();
			m_thrustBackwardCommand		= new ThrustBackwardCommand();
			m_rollLeftCommand 				= new RollLeftCommand();
			m_rollRightCommand 				= new RollRightCommand();
			m_pitchForwardCommand 		= new PitchForwardCommand();
			m_pitchBackwardCommand 		= new PitchBackwardCommand();
			m_headLeftCommand					= new HeadLeftCommand();
			m_headRightCommand				= new HeadRightCommand();
			m_thrustUpCommand					= new ThrustUpCommand();
			m_thrustDownCommand				= new ThrustDownCommand();
			m_thrustLeftCommand				= new ThrustLeftCommand();
			m_thrustRightCommand			= new ThrustRightCommand();

			m_initialized = xdl::xdl_true;

			return xdl::ERR_OK;
		}

		xdl::xdl_int SpaceShipInputHandler::redirect(xdl::xdl_bool state) {
			if(state) {
				m_kd->getButton(xdl::KEY_UP, 				&m_pitchUp);
				m_kd->getButton(xdl::KEY_DOWN, 			&m_pitchDown);
				m_kd->getButton(xdl::KEY_Q,	 				&m_rollLeft);
				m_kd->getButton(xdl::KEY_E,					&m_rollRight);
				m_kd->getButton(xdl::KEY_LEFT, 			&m_headLeft);
				m_kd->getButton(xdl::KEY_RIGHT,			&m_headRight);
				m_kd->getButton(xdl::KEY_LSHIFT,		&m_thrust);
				m_kd->getButton(xdl::KEY_LALT,			&m_thrustBackward);
				m_kd->getButton(xdl::KEY_PAGEUP,		&m_thrustUp);
				m_kd->getButton(xdl::KEY_PAGEDOWN,	&m_thrustDown);
				m_kd->getButton(xdl::KEY_Z,					&m_thrustLeft);
				m_kd->getButton(xdl::KEY_X,					&m_thrustRight);

			} else {
				m_kd->getButton(xdl::KEY_UP, 				&m_pitchUp);
				m_kd->getButton(xdl::KEY_DOWN, 			&m_pitchDown);
				m_kd->getButton(xdl::KEY_Q,	 				&m_rollLeft);
				m_kd->getButton(xdl::KEY_E,					&m_rollRight);
				m_kd->getButton(xdl::KEY_LEFT, 			&m_headLeft);
				m_kd->getButton(xdl::KEY_RIGHT,			&m_headRight);
				m_kd->getButton(xdl::KEY_W,					&m_thrust);
				m_kd->getButton(xdl::KEY_S,					&m_thrustBackward);
				m_kd->getButton(xdl::KEY_SPACE,			&m_thrustUp);
				m_kd->getButton(xdl::KEY_C,					&m_thrustDown);
				m_kd->getButton(xdl::KEY_A,					&m_thrustLeft);
				m_kd->getButton(xdl::KEY_D,					&m_thrustRight);
			}
			return xdl::ERR_OK;
		}


		void SpaceShipInputHandler::handleInput(SpaceShip* actor, xdl::xdl_double dT) {
			assert(m_initialized && "GameInputHandler::handleInput: Not initialized.");
			if(actor == nullptr) {
				return;
			}

			btVector3 av = actor->getRigidBody()->getAngularVelocity();
			btVector3 sum = av;

			xdl::xdl_bool rollActivated = xdl::xdl_false;

			//
			// Roll here.
			//
			if(m_rollLeft->getPressed()) {
				m_rollLeftCommand->execute(actor, dT);
				rollActivated = xdl::xdl_true;
			}
			if(m_rollRight->getPressed()) {
				m_rollRightCommand->execute(actor, dT);
				rollActivated =xdl::xdl_true;
			}

			if(rollActivated == xdl::xdl_false) {
				sum += compensateRollDrift(actor, dT);
			}

			//
			// Pitch here.
			//
			xdl::xdl_bool pitchActivated = xdl::xdl_false;
			if(m_pitchDown->getPressed()) {
				m_pitchForwardCommand->execute(actor, dT);
				pitchActivated =xdl::xdl_true;
			}
			if(m_pitchUp->getPressed()) {
				m_pitchBackwardCommand->execute(actor, dT);
				pitchActivated =xdl::xdl_true;
			}

			if(pitchActivated == xdl::xdl_false) {
				sum += compensatePitchDrift(actor, dT);
			}

			//
			// Heading here.
			//
			xdl::xdl_bool headingActivated = xdl::xdl_false;
			if(m_headLeft->getPressed()) {
				m_headLeftCommand->execute(actor, dT);
				headingActivated = xdl::xdl_true;
			}
			if(m_headRight->getPressed()) {
				m_headRightCommand->execute(actor, dT);
				headingActivated = xdl::xdl_true;
			}

			if(headingActivated == xdl::xdl_false) {
				sum += compensateHeadingDrift(actor, dT);
			}


			//
			// Thrust here.
			//
			if(m_thrust->getPressed()) {
				m_thrustCommand->execute(actor, dT);
			}
			if(m_thrustBackward->getPressed()) {
				m_thrustBackwardCommand->execute(actor, dT);
			}
			if(m_thrustDown->getPressed()) {
				m_thrustDownCommand->execute(actor, dT);
			}
			if(m_thrustUp->getPressed()) {
				m_thrustUpCommand->execute(actor, dT);
			}
			if(m_thrustLeft->getPressed()) {
				m_thrustLeftCommand->execute(actor, dT);
			}
			if(m_thrustRight->getPressed()) {
				m_thrustRightCommand->execute(actor, dT);
			}



			actor->getRigidBody()->setAngularVelocity(sum);

		}

		btVector3 SpaceShipInputHandler::compensateRollDrift(SpaceShip* actor, double dT) {
			//
			// Compansate roll drift.
			//
			btVector3 rollAxes(btVector3(0.0f, 0.0f, -1.0));
			rollAxes = quatRotate(actor->getRigidBody()->getOrientation(), rollAxes);
			rollAxes.normalize();

			btScalar dampingRoll = actor->getRigidBody()->getAngularVelocity().dot(rollAxes);

			return btVector3(-0.02 * dampingRoll * rollAxes);
		}

		btVector3 SpaceShipInputHandler::compensateHeadingDrift(SpaceShip* actor, double dT) {
			//
			// Compansate heading drift.
			//
			btVector3 headingAxes(btVector3(0.0f, 1.0f, 0.0));
			headingAxes = quatRotate(actor->getRigidBody()->getOrientation(), headingAxes);
			headingAxes.normalize();

			btScalar dampingHeading = actor->getRigidBody()->getAngularVelocity().dot(headingAxes);

			return btVector3(-0.02 * dampingHeading * headingAxes);
		}

		btVector3 SpaceShipInputHandler::compensatePitchDrift(SpaceShip* actor, double dT) {
			//
			// Compansate heading drift.
			//
			btVector3 pitchAxes(btVector3(1.0f, 0.0f, 0.0));
			pitchAxes = quatRotate(actor->getRigidBody()->getOrientation(), pitchAxes);
			pitchAxes.normalize();

			btScalar dampingPitch = actor->getRigidBody()->getAngularVelocity().dot(pitchAxes);

			return btVector3(-0.02 * dampingPitch * pitchAxes);
		}

	}

}
