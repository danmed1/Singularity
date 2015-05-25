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

#ifndef GAME_INPUT_HANDLER_H
#define GAME_INPUT_HANDLER_H


#include <XdevLMouse/XdevLMouse.h>
#include <XdevLKeyboard/XdevLKeyboard.h>
#include <XdevLJoystick/XdevLJoystick.h>

#include <Game/SpaceShipCommands.h>

namespace soan {
	
	namespace game {
		
		class SpaceShipInputHandler {
			public:
				SpaceShipInputHandler(xdl::XdevLKeyboard* kd,
				                  xdl::XdevLMouse* md,
				                  xdl::XdevLJoystick* jd);
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
				xdl::XdevLKeyboard* 				m_kd;
				xdl::XdevLMouse* 					m_md;
				xdl::XdevLJoystick* 				m_jd;

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
				xdl::XdevLAxis*							m_headingAxis;
				xdl::XdevLAxis*							m_pitchAxis;

				ThrustCommand*							m_thrustCommand;
				ThrustBackwardCommand*			m_thrustBackwardCommand;
				RollLeftCommand*						m_rollLeftCommand;
				RollRightCommand*						m_rollRightCommand;
				PitchCommand*							m_pitchCommand;
				PitchForwardCommand* 				m_pitchForwardCommand;
				PitchBackwardCommand*				m_pitchBackwardCommand;
				HeadingCommand*						m_headingCommand;
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
