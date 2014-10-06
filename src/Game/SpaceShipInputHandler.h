#ifndef GAME_INPUT_HANDLER_H
#define GAME_INPUT_HANDLER_H


#include <XdevLMouseDevice/XdevLMouseDevice.h>
#include <XdevLKeyboardDevice/XdevLKeyboardDevice.h>
#include <XdevLJoystickDevice/XdevLJoystickDevice.h>

#include <Game/SpaceShipCommands.h>

namespace soan {
	
	namespace game {
		
		class SpaceShipInputHandler {
			public:
				SpaceShipInputHandler(xdl::XdevLKeyboardDevice* kd,
				                  xdl::XdevLMouseDevice* md,
				                  xdl::XdevLJoystickDevice* jd);
				~SpaceShipInputHandler();

				/// Initialize the handler.
				xdl::xdl_int init();
				
				/// Handle incomming inputs.
				void handleInput(SpaceShip* actor, xdl::xdl_double dT);

				/// Redirect input events.
				xdl::xdl_int redirect(xdl::xdl_bool state);

			private:
				btVector3 compensateRollDrift(SpaceShip* actor, double dT);
				btVector3 compensateHeadingDrift(SpaceShip* actor, double dT);
				btVector3 compensatePitchDrift(SpaceShip* actor, double dT);
				
			private:
				xdl::XdevLKeyboardDevice* 	m_kd;
				xdl::XdevLMouseDevice* 			m_md;
				xdl::XdevLJoystickDevice* 	m_jd;

				xdl::XdevLButton*						m_thrust;
				xdl::XdevLButton*						m_thrustBackward;				
				xdl::XdevLButton*						m_pitchUp;
				xdl::XdevLButton*						m_pitchDown;
				xdl::XdevLButton*						m_rollLeft;
				xdl::XdevLButton*						m_rollRight;
				xdl::XdevLButton*						m_headLeft;
				xdl::XdevLButton*						m_headRight;
				xdl::XdevLButton*						m_thrustUp;
				xdl::XdevLButton*						m_thrustDown;
				xdl::XdevLButton*						m_thrustLeft;
				xdl::XdevLButton*						m_thrustRight;
				
				xdl::XdevLAxis*							m_thrustAxis;

				ThrustCommand*							m_thrustCommand;
				ThrustBackwardCommand*			m_thrustBackwardCommand;
				RollLeftCommand*						m_rollLeftCommand;
				RollRightCommand*						m_rollRightCommand;
				PitchForwardCommand* 				m_pitchForwardCommand;
				PitchBackwardCommand*				m_pitchBackwardCommand;
				HeadLeftCommand*						m_headLeftCommand;
				HeadRightCommand*						m_headRightCommand;
				ThrustUpCommand*						m_thrustUpCommand;
				ThrustDownCommand*					m_thrustDownCommand;
				ThrustLeftCommand*					m_thrustLeftCommand;
				ThrustRightCommand*					m_thrustRightCommand;
				
				xdl::xdl_bool 							m_initialized;
		};
		
	}
	
}

#endif // GAME_INPUT_HANDLER_H
