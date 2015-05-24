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

#ifndef COMMAND_H
#define COMMAND_H

#include <Game/SpaceShip.h>

namespace soan {

	namespace game {

		class Command {
			public:

				virtual ~Command() {};

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) = 0;
				virtual void undo() = 0;

		};

		class ThrustCommand : public Command {
			public:
				virtual ~ThrustCommand() {}

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) override {

					//
					// What is this here?
					// If we would use real physics the actor would move in a direction until
					// the velocity into that direction is canceled. Because controlling something
					// with that behaviour is difficult we use a trick here.
					// We apply an impulse to that object. After that we get the linear velocity and
					// remove all parts which is not in the forward direction of the ship and
					// set it back to the actor. Now it gets easier to control.
					//
					btVector3 thrust(btVector3(0.0f, 0.0f, -1.0));
					thrust = quatRotate(actor->getRigidBody()->getOrientation(), thrust);
					thrust.normalize();


					actor->getRigidBody()->applyCentralImpulse(actor->getCurrentThrustImpulse()*thrust);

					btVector3 side(btVector3(-1.0f, 0.0f, 0.0));
					side = quatRotate(actor->getRigidBody()->getOrientation(), side);
					side.normalize();

					btScalar sideVel = actor->getRigidBody()->getLinearVelocity().dot(side);
					actor->getRigidBody()->applyCentralImpulse(-side*sideVel);



					//	btScalar lvforwardDirection = actor->getRigidBody()->getLinearVelocity().length();


					//	actor->getRigidBody()->setLinearVelocity(lvforwardDirection*thrust);

				}
				virtual void undo() override {}
		};

		class ThrustBackwardCommand : public Command {
			public:
				virtual ~ThrustBackwardCommand() {}

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) override {

					btVector3 thrust(btVector3(0.0f, 0.0f, 1.0));
					thrust = quatRotate(actor->getRigidBody()->getOrientation(), thrust);
					thrust.normalize();

					actor->getRigidBody()->applyCentralImpulse(actor->getCurrentThrustImpulse()*thrust);

				}
				virtual void undo() override {}
		};

		class RollLeftCommand : public Command {
			public:
				virtual ~RollLeftCommand() {}

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) override {
					btVector3 roll(btVector3(0.0f, 0.0f, actor->getMaxRollImpulse()));
					roll = quatRotate(actor->getRigidBody()->getOrientation(), roll);
					actor->getRigidBody()->applyTorque(roll);
				}
				virtual void undo() override {}
		};

		class RollRightCommand : public Command {
			public:
				virtual ~RollRightCommand() {}

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) override {
					btVector3 roll(btVector3(0.0f, 0.0f, -actor->getMaxRollImpulse()));
					roll = quatRotate(actor->getRigidBody()->getOrientation(), roll);
					actor->getRigidBody()->applyTorque(roll);
				}

				virtual void undo() override {}
		};

		class PitchForwardCommand : public Command {
			public:
				virtual ~PitchForwardCommand() {}

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) override {
					btVector3 pitch(btVector3(actor->getMaxPitchImpulse(), 0.0f, 0.0f));
					pitch = quatRotate(actor->getRigidBody()->getOrientation(), pitch);
					actor->getRigidBody()->applyTorque(pitch);
				}

				virtual void undo() override {}
		};

		class PitchBackwardCommand : public Command {
			public:
				virtual ~PitchBackwardCommand() {}

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) override {
					btVector3 pitch(btVector3(-actor->getMaxPitchImpulse(), 0.0f, 0.0f));
					pitch = quatRotate(actor->getRigidBody()->getOrientation(), pitch);
					actor->getRigidBody()->applyTorque(pitch);
				}

				virtual void undo() override {}
		};

		class HeadLeftCommand : public Command {
			public:
				virtual ~HeadLeftCommand() {}

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) override {
					btVector3 heading(btVector3(0.0f, actor->getMaxTurnImpulse(), 0.0f));
					heading = quatRotate(actor->getRigidBody()->getOrientation(), heading);
					actor->getRigidBody()->applyTorque(heading);
				}

				virtual void undo() override {}
		};

		class HeadRightCommand : public Command {
			public:
				virtual ~HeadRightCommand() {}

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) override {
					btVector3 heading(btVector3(0.0f, -actor->getMaxTurnImpulse(), 0.0f));
					heading = quatRotate(actor->getRigidBody()->getOrientation(), heading);
					actor->getRigidBody()->applyTorque(heading);
				}

				virtual void undo() override {}
		};

		class ThrustUpCommand : public Command {
			public:
				virtual ~ThrustUpCommand() {}

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) override {
					btVector3 thrust(btVector3(0.0f, 1.0f, 0.0));
					thrust = quatRotate(actor->getRigidBody()->getOrientation(), thrust);
					thrust.normalize();

					actor->getRigidBody()->applyCentralImpulse(actor->getMaxThrustImpulse()*thrust);


				}

				virtual void undo() override {}
		};

		class ThrustDownCommand : public Command {
			public:
				virtual ~ThrustDownCommand() {}

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) override {
					btVector3 thrust(btVector3(0.0f, -1.0f, 0.0));
					thrust = quatRotate(actor->getRigidBody()->getOrientation(), thrust);
					thrust.normalize();

					actor->getRigidBody()->applyCentralImpulse(actor->getMaxThrustImpulse()*thrust);

				}

				virtual void undo() override {}
		};

		class ThrustLeftCommand : public Command {
			public:
				virtual ~ThrustLeftCommand() {}

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) override {
					btVector3 thrust(btVector3(-1.0f, 0.0f, 0.0));
					thrust = quatRotate(actor->getRigidBody()->getOrientation(), thrust);
					thrust.normalize();

					actor->getRigidBody()->applyCentralImpulse(actor->getMaxThrustImpulse()*thrust);


				}

				virtual void undo() override {}
		};

		class ThrustRightCommand : public Command {
			public:
				virtual ~ThrustRightCommand() {}

				virtual void execute(SpaceShip* actor, xdl::xdl_double dT) override {
					btVector3 thrust(btVector3(1.0f, 0.0f, 0.0));
					thrust = quatRotate(actor->getRigidBody()->getOrientation(), thrust);
					thrust.normalize();

					actor->getRigidBody()->applyCentralImpulse(actor->getMaxThrustImpulse()*thrust);


				}

				virtual void undo() override {}
		};

	}
}

#endif
